//#include "PCSSApplication/PCSSApplication.h"
//#include "HelloBoxApplication/HelloBoxApplication.h"
//#include "IBLApplication/IBLApplication.h"
//#include "FFTBloomApplication/FFTBloomApplication.h"
//#include "SSRApplication/SSRApplication.h"
#include "SSAOApplication/SSAOApplication.h"

int main() {
    SSAOApplication app{};
    return app.execute();
}

