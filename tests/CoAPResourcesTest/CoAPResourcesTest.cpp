#include "Test.hpp"

Array<String> prepareSpeakerResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_SPEAKER);
    res.insert(uri_path, new unsigned short(RADIO_SPEAKER));
    return uri_path;
}

Array<String> prepareLampResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_REMOTE);
    uri_path.pushBack(RESOURCE_LAMP);
    res.insert(uri_path, new unsigned short(RADIO_LAMP));
    return uri_path;
}

Array<String> prepareRttResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_RTT);
    res.insert(uri_path, nullptr);
    return uri_path;
}

Array<String> prepareJitterResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_JITTER);
    res.insert(uri_path, nullptr);
    return uri_path;
}

Array<String> prepareTimedOutResource(CoAPResources &res) {
    Array<String> uri_path;
    uri_path.pushBack(RESOURCE_LOCAL);
    uri_path.pushBack(RESOURCE_TIMED_OUT);
    res.insert(uri_path, nullptr);
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
        coapResources.insert(strings1, nullptr);
        coapResources.insert(strings2, nullptr);

        String actual1 = coapResources.search(strings1)->getKey();
        String actual2 = coapResources.search(strings2)->getKey();

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

        assertEqual(*(speaker.end()-1), coapResources.search(speaker)->getKey());
        assertEqual(*(lamp.end()-1), coapResources.search(lamp)->getKey());
        assertEqual(*(jitter.end()-1), coapResources.search(jitter)->getKey());
        assertEqual(*(rtt.end()-1), coapResources.search(rtt)->getKey());
        assertEqual(*(timed_out.end()-1), coapResources.search(timed_out)->getKey());

        assertEqual(coapResources.toLinkFormat(), "</remote/speaker>;value=1,</remote/lamp>;value=0,</local/jitter>,</local/rtt>,</local/timed_out>");
    }

endTest