#include "Test.hpp"

Array<String> prepareSpeakerResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_SPEAKER);
    res.insert(uri_path, RADIO_SPEAKER);
    return uri_path;
}

Array<String> prepareLampResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_LAMP);
    res.insert(uri_path, RADIO_LAMP);
    return uri_path;
}

Array<String> prepareRttResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_RTT);
    res.insert(uri_path, 0);
    return uri_path;
}

Array<String> prepareJitterResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_JITTER);
    res.insert(uri_path, 0);
    return uri_path;
}

Array<String> prepareTimedOutResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_TIMED_OUT);
    res.insert(uri_path, 0);
    return uri_path;
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

    test (AllResources) {
        CoAPResources coapResources;

        Array<String> speaker = prepareSpeakerResource(coapResources);
        Array<String> lamp = prepareLampResource(coapResources);
        Array<String> jitter = prepareJitterResource(coapResources);
        Array<String> rtt = prepareRttResource(coapResources);
        Array<String> timed_out = prepareTimedOutResource(coapResources);

        assertEqual(*(speaker.end()-1), coapResources.search(speaker)->key);
        assertEqual(*(lamp.end()-1), coapResources.search(lamp)->key);
        assertEqual(*(jitter.end()-1), coapResources.search(jitter)->key);
        assertEqual(*(rtt.end()-1), coapResources.search(rtt)->key);
        assertEqual(*(timed_out.end()-1), coapResources.search(timed_out)->key);

        assertEqual(coapResources.toLinkFormat(), "</remote/speaker>;value=1,</remote/lamp>;value=0,</local/jitter>;value=0,</local/rtt>;value=0,</local/timed_out>;value=0");
    }

endTest