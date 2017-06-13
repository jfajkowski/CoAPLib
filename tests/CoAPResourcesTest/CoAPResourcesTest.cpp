#include "Test.hpp"

beginTest

    test(Create) {
        String expected1 = "3a";
        String expected2 = "3b";

        Array<String> strings1;
        strings1.pushBack("1");
        strings1.pushBack("2");
        strings1.pushBack("3a");

        Array<String> strings2;
        strings2.pushBack("1");
        strings2.pushBack("2");
        strings2.pushBack("3b");

        CoAPResources coapResources;
        coapResources.insert(strings1, 0);
        coapResources.insert(strings2, 0);

        String actual1 = coapResources.search(strings1)->key;
        String actual2 = coapResources.search(strings2)->key;

        assertEqual(expected1, actual1);
        assertEqual(expected2, actual2);
    }

    test(FindCore) {
        Array<String> coreResource;
        coreResource.pushBack(".well-known");
        coreResource.pushBack("core");

        CoAPResources coapResources;
        assert(coapResources.search(coreResource) != nullptr);
    }

endTest