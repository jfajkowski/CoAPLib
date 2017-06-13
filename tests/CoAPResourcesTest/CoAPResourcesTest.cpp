#include "Test.hpp"

void prepareSpeakerResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack("remote");
    uri_path.pushBack("speaker");
    res.insert(uri_path, SPEAKER);
}

void prepareLampResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack("remote");
    uri_path.pushBack("lamp");
    res.insert(uri_path, LAMP);
}

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

        prepareSpeakerResource(coapResources);
        prepareLampResource(coapResources);

        assert(coapResources.search(coreResource) != nullptr);
    }

    test(ToCoreFormat) {
        CoAPResources coapResources;

        prepareSpeakerResource(coapResources);
        prepareLampResource(coapResources);

        assertEqual(coapResources.toLinkFormat(), String("</remote/speaker>;value=1,</remote/lamp>;value=0"));
    }

endTest