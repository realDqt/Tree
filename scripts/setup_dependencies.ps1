[CmdletBinding()]
param()

Set-StrictMode -Version Latest
$ErrorActionPreference = "Stop"

$projectRoot = Split-Path -Parent $PSScriptRoot
$thirdPartyRoot = Join-Path $projectRoot "third_party"
$downloadCache = Join-Path $thirdPartyRoot ".cache"

New-Item -ItemType Directory -Force -Path $thirdPartyRoot, $downloadCache | Out-Null

function Invoke-GitStep {
    param(
        [Parameter(Mandatory)]
        [string[]] $Arguments,

        [Parameter(Mandatory)]
        [string] $Description
    )

    & git @Arguments
    if ($LASTEXITCODE -ne 0) {
        throw "$Description failed with exit code $LASTEXITCODE."
    }
}

function Install-GitDependency {
    param(
        [Parameter(Mandatory)]
        [string] $Name,

        [Parameter(Mandatory)]
        [string] $Repository,

        [Parameter(Mandatory)]
        [string] $Commit,

        [Parameter(Mandatory)]
        [string] $Directory,

        [Parameter(Mandatory)]
        [string] $Marker
    )

    $destination = Join-Path $thirdPartyRoot $Directory
    $markerPath = Join-Path $destination $Marker

    if (Test-Path -LiteralPath $markerPath) {
        if (-not (Test-Path -LiteralPath (Join-Path $destination ".git"))) {
            throw "$Name exists at $destination, but its pinned revision cannot be verified because .git is missing."
        }

        $actualCommit = (& git -C $destination rev-parse HEAD).Trim()
        if ($LASTEXITCODE -ne 0 -or $actualCommit -ne $Commit) {
            throw "$Name exists at $destination but is not at pinned commit $Commit."
        }

        $workingTreeChanges = @(& git -C $destination status --porcelain --untracked-files=all)
        if ($LASTEXITCODE -ne 0 -or $workingTreeChanges.Count -ne 0) {
            throw "$Name has modified tracked files at $destination."
        }

        Write-Host "$Name is already available."
        return
    }

    if (Test-Path -LiteralPath $destination) {
        throw "$Name has an incomplete directory at $destination. Remove it and run this script again."
    }

    Write-Host "Downloading $Name..."
    $temporaryDestination = "$destination.download-$([Guid]::NewGuid().ToString('N'))"

    try {
        New-Item -ItemType Directory -Path $temporaryDestination | Out-Null
        Invoke-GitStep -Arguments @("-C", $temporaryDestination, "init", "--quiet") -Description "Initializing $Name"
        Invoke-GitStep -Arguments @("-C", $temporaryDestination, "remote", "add", "origin", $Repository) -Description "Adding the $Name remote"
        Invoke-GitStep -Arguments @("-C", $temporaryDestination, "fetch", "--depth", "1", "origin", $Commit) -Description "Downloading $Name"
        Invoke-GitStep -Arguments @("-C", $temporaryDestination, "checkout", "--detach", "--quiet", "FETCH_HEAD") -Description "Checking out $Name"

        $actualCommit = (& git -C $temporaryDestination rev-parse HEAD).Trim()
        if ($LASTEXITCODE -ne 0 -or $actualCommit -ne $Commit) {
            throw "$Name checkout verification failed. Expected $Commit, got $actualCommit."
        }

        if (-not (Test-Path -LiteralPath (Join-Path $temporaryDestination $Marker))) {
            throw "$Name checkout is missing required file: $Marker"
        }

        Move-Item -LiteralPath $temporaryDestination -Destination $destination
    } finally {
        if (Test-Path -LiteralPath $temporaryDestination) {
            $resolvedTemporaryPath = [IO.Path]::GetFullPath($temporaryDestination)
            $resolvedThirdPartyRoot = [IO.Path]::GetFullPath($thirdPartyRoot) + [IO.Path]::DirectorySeparatorChar
            if (-not $resolvedTemporaryPath.StartsWith($resolvedThirdPartyRoot, [StringComparison]::OrdinalIgnoreCase)) {
                throw "Refusing to clean a temporary directory outside third_party: $resolvedTemporaryPath"
            }
            Remove-Item -LiteralPath $resolvedTemporaryPath -Recurse -Force
        }
    }
}

function Test-VulkanSdk {
    param(
        [Parameter(Mandatory)]
        [string] $Root
    )

    $requiredSdkFiles = @(
        (Join-Path $Root "Include\vulkan\vulkan.h"),
        (Join-Path $Root "Include\vulkan\vulkan_core.h"),
        (Join-Path $Root "Lib\vulkan-1.lib"),
        (Join-Path $Root "Bin\glslangValidator.exe"),
        (Join-Path $Root "Bin\VkLayer_khronos_validation.json"),
        (Join-Path $Root "Bin\VkLayer_khronos_validation.dll")
    )

    foreach ($requiredSdkFile in $requiredSdkFiles) {
        if (-not (Test-Path -LiteralPath $requiredSdkFile)) {
            return $false
        }
    }

    $coreHeader = Get-Content -LiteralPath (Join-Path $Root "Include\vulkan\vulkan_core.h") -Raw
    return $coreHeader -match "#define\s+VK_HEADER_VERSION\s+328(?:\r?\n)" -and
        $coreHeader -match "VK_MAKE_API_VERSION\(0,\s*1,\s*4,\s*VK_HEADER_VERSION\)"
}

$gitDependencies = @(
    @{
        Name = "GLFW 3.4"
        Repository = "https://github.com/glfw/glfw.git"
        Commit = "7b6aead9fb88b3623e3b3725ebb42670cbe4c579"
        Directory = "glfw"
        Marker = "CMakeLists.txt"
    },
    @{
        Name = "Assimp 5.4.3"
        Repository = "https://github.com/assimp/assimp.git"
        Commit = "c35200e38ea8f058812b83de2ef32c6093b0ece2"
        Directory = "assimp"
        Marker = "CMakeLists.txt"
    },
    @{
        Name = "GLM 1.0.1"
        Repository = "https://github.com/g-truc/glm.git"
        Commit = "0af55ccecd98d4e5a8d1fad7de25ba429d60e863"
        Directory = "glm"
        Marker = "glm/glm.hpp"
    },
    @{
        Name = "tinyobjloader 2.0.0-rc13"
        Repository = "https://github.com/tinyobjloader/tinyobjloader.git"
        Commit = "2945a967c5303b2c8c14174117c45f3302591150"
        Directory = "tinyobjloader"
        Marker = "tiny_obj_loader.h"
    },
    @{
        Name = "stb"
        Repository = "https://github.com/nothings/stb.git"
        Commit = "2c980bb59875b0d32144a71867fbdebb2f77cd20"
        Directory = "stb"
        Marker = "stb_image.h"
    }
)

foreach ($dependency in $gitDependencies) {
    Install-GitDependency @dependency
}

$vulkanVersion = "1.4.328.1"
$vulkanSha256 = "A8675DF6D538079C2A719A9373994948091DB785B48F142E024254E76348D16C"
$vulkanRoot = Join-Path $thirdPartyRoot "VulkanSDK"
$vulkanHeader = Join-Path $vulkanRoot "Include\vulkan\vulkan.h"
$vulkanLibrary = Join-Path $vulkanRoot "Lib\vulkan-1.lib"

if (Test-VulkanSdk -Root $vulkanRoot) {
    Write-Host "Vulkan SDK $vulkanVersion is already available."
} else {
    if (Test-Path -LiteralPath $vulkanRoot) {
        throw "Vulkan SDK at $vulkanRoot is incomplete or is not version $vulkanVersion. Remove it and run this script again."
    }

    $installerName = "vulkansdk-windows-X64-$vulkanVersion.exe"
    $installerPath = Join-Path $downloadCache $installerName
    $installerUrl = "https://sdk.lunarg.com/sdk/download/$vulkanVersion/windows/vulkan-sdk.exe"

    $downloadInstaller = $true
    if (Test-Path -LiteralPath $installerPath) {
        $downloadInstaller = (Get-FileHash -Algorithm SHA256 -LiteralPath $installerPath).Hash -ne $vulkanSha256
        if ($downloadInstaller) {
            Write-Warning "The cached Vulkan SDK installer is incomplete or invalid; it will be replaced."
        }
    }

    if ($downloadInstaller) {
        Write-Host "Downloading Vulkan SDK $vulkanVersion..."
        $temporaryInstaller = "$installerPath.partial-$([Guid]::NewGuid().ToString('N'))"
        try {
            & curl.exe -fL --retry 3 --output $temporaryInstaller $installerUrl
            if ($LASTEXITCODE -ne 0) {
                throw "Downloading Vulkan SDK failed with exit code $LASTEXITCODE."
            }

            $downloadedHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $temporaryInstaller).Hash
            if ($downloadedHash -ne $vulkanSha256) {
                throw "Vulkan SDK checksum mismatch. Expected $vulkanSha256, got $downloadedHash."
            }

            Move-Item -LiteralPath $temporaryInstaller -Destination $installerPath -Force
        } finally {
            if (Test-Path -LiteralPath $temporaryInstaller) {
                Remove-Item -LiteralPath $temporaryInstaller -Force
            }
        }
    }

    $actualHash = (Get-FileHash -Algorithm SHA256 -LiteralPath $installerPath).Hash
    if ($actualHash -ne $vulkanSha256) {
        throw "Vulkan SDK checksum mismatch. Expected $vulkanSha256, got $actualHash."
    }

    $signature = Get-AuthenticodeSignature -LiteralPath $installerPath
    if ($signature.Status -ne "Valid" -or $signature.SignerCertificate.Subject -notmatch "LunarG") {
        throw "Vulkan SDK installer does not have a valid LunarG signature."
    }

    $temporaryVulkanRoot = "$vulkanRoot.install-$([Guid]::NewGuid().ToString('N'))"
    try {
        Write-Host "Installing Vulkan SDK $vulkanVersion in copy-only mode..."
        & $installerPath --root $temporaryVulkanRoot --accept-licenses --default-answer --confirm-command install copy_only=1
        if ($LASTEXITCODE -ne 0) {
            throw "Vulkan SDK installation failed with exit code $LASTEXITCODE."
        }

        if (-not (Test-VulkanSdk -Root $temporaryVulkanRoot)) {
            throw "Vulkan SDK installation completed without all required version $vulkanVersion files."
        }

        Move-Item -LiteralPath $temporaryVulkanRoot -Destination $vulkanRoot
    } finally {
        if (Test-Path -LiteralPath $temporaryVulkanRoot) {
            $resolvedTemporaryPath = [IO.Path]::GetFullPath($temporaryVulkanRoot)
            $resolvedThirdPartyRoot = [IO.Path]::GetFullPath($thirdPartyRoot) + [IO.Path]::DirectorySeparatorChar
            if (-not $resolvedTemporaryPath.StartsWith($resolvedThirdPartyRoot, [StringComparison]::OrdinalIgnoreCase)) {
                throw "Refusing to clean a temporary directory outside third_party: $resolvedTemporaryPath"
            }
            Remove-Item -LiteralPath $resolvedTemporaryPath -Recurse -Force
        }
    }
}

$requiredFiles = @(
    (Join-Path $thirdPartyRoot "glfw\CMakeLists.txt"),
    (Join-Path $thirdPartyRoot "assimp\CMakeLists.txt"),
    (Join-Path $thirdPartyRoot "glm\glm\glm.hpp"),
    (Join-Path $thirdPartyRoot "stb\stb_image.h"),
    (Join-Path $thirdPartyRoot "stb\stb_image_write.h"),
    (Join-Path $thirdPartyRoot "tinyobjloader\tiny_obj_loader.h"),
    $vulkanHeader,
    $vulkanLibrary,
    (Join-Path $vulkanRoot "Bin\glslangValidator.exe"),
    (Join-Path $vulkanRoot "Bin\VkLayer_khronos_validation.json"),
    (Join-Path $vulkanRoot "Bin\VkLayer_khronos_validation.dll")
)

foreach ($requiredFile in $requiredFiles) {
    if (-not (Test-Path -LiteralPath $requiredFile)) {
        throw "Dependency setup finished without required file: $requiredFile"
    }
}

Write-Host "All dependencies are ready in $thirdPartyRoot."
