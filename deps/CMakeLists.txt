CPMAddPackage(gh:gabime/spdlog@1.15.0)
CPMAddPackage(gh:Curve/rohrkabel@8.0)
CPMAddPackage(gh:Neargye/magic_enum@0.9.7)
CPMAddPackage(gh:victimsnino/ReactivePlusPlus@2.2.0)
CPMAddPackage(gh:nlohmann/json@3.11.3)
CPMAddPackage("gh:vector-of-bool/cmrc#952ffdd")
CPMAddPackage(
    NAME Drogon
    GITHUB_REPOSITORY drogonframework/drogon
    VERSION 1.9.8
    OPTIONS
        "BUILD_CTL OFF"
        "BUILD_EXAMPLES OFF"
        "BUILD_ORM OFF"
        "BUILD_BROTLI OFF"
        "BUILD_YAML_CONFIG OFF"
        "BUILD_TESTS OFF"
        "BUILD_TESTING OFF"
)
