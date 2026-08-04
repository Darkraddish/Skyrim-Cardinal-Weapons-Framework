#include "CWFramework/PublicAPI.h"
#include "CWFramework/SchemaValidator.h"
#include <iostream>
#include <cassert>

using namespace CWFramework;

int main()
{
    std::cout << "=== Running C++ Zero-Content Load Verification Test ===" << std::endl;

    // Clear registry to ensure 0 state
    Registry::GetInstance().Clear();

    // 1. Verify Framework Version and Status
    std::cout << "Framework Version: " << PublicAPI::GetVersion() << std::endl;
    assert(PublicAPI::IsFrameworkReady() == true);
    std::cout << "[PASS] Framework is ready." << std::endl;

    // 2. Verify Zero-Content Load Status
    assert(PublicAPI::IsZeroContentLoaded() == true);
    assert(PublicAPI::GetRegisteredWeaponTypeCount() == 0);
    assert(Registry::GetInstance().GetSeriesCount() == 0);
    assert(Registry::GetInstance().GetWeaponFormCount() == 0);
    assert(Registry::GetInstance().GetProgressionNodeCount() == 0);
    std::cout << "[PASS] Zero-content state verified: 0 weapon types, 0 series, 0 forms, 0 nodes registered." << std::endl;

    // 3. Verify Decoupled Data Registration & Schema Validation
    WeaponTypeData swordType;
    swordType.id = "CW_WT_SWORD";
    swordType.name = "Cardinal Sword";
    swordType.description = "Legendary Sword progression category.";
    
    auto valRes = SchemaValidator::ValidateWeaponType(swordType);
    assert(valRes.isValid == true);
    std::cout << "[PASS] SchemaValidator passed valid WeaponType data." << std::endl;

    bool regResult = PublicAPI::RegisterWeaponType(swordType);
    assert(regResult == true);
    assert(PublicAPI::GetRegisteredWeaponTypeCount() == 1);
    assert(PublicAPI::IsZeroContentLoaded() == false);
    std::cout << "[PASS] Successfully registered WeaponType '" << swordType.id << "'." << std::endl;

    // 4. Reset & verify clear
    Registry::GetInstance().Clear();
    assert(PublicAPI::IsZeroContentLoaded() == true);
    std::cout << "[PASS] Registry reset successfully." << std::endl;

    std::cout << "\nSUCCESS: All Phase 1 C++ Core Tests Passed!" << std::endl;
    return 0;
}
