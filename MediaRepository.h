#pragma once

#include <rohrkabel/node/node.hpp>

#include <rpp/observables/dynamic_observable.hpp>
#include <rpp/subjects/publish_subject.hpp>

#include "MediaClass.h"
#include "Repository.h"

template <MediaClass C>
class MediaRepository {
public:
    MediaRepository(Repository<pw::node>& nodeRepository) {
        nodeRepository.proxyAdded().subscribe([&](const pw::node& node) {
            if (!isMediaClass(node)) return;

            _media.emplace(node.id(), nodeRepository.bind(node.id()));
            _mediaAddedSubject.get_observer().on_next(node);
        });

        nodeRepository.proxyRemoved().subscribe([this](const pw::node& node) {
            if (!_media.contains(node.id())) return;

            _media.erase(node.id());
            _mediaRemovedSubject.get_observer().on_next(node);
        });
    }

    inline const std::map<std::uint32_t, pw::node>& media() const {
        return _media;
    }

    inline const rpp::dynamic_observable<pw::node> mediaAdded() const {
        return _mediaAddedSubject.get_observable();
    }

    inline const rpp::dynamic_observable<pw::node> mediaRemoved() const {
        return _mediaRemovedSubject.get_observable();
    }

private:
    static bool isMediaClass(const pw::node& node) {
        if (!node.props().contains("media.class")) return false;
        if (node.props().at("media.class") != toString(C)) return false;

        return true;
    }

    std::map<std::uint32_t, pw::node> _media;

    rpp::subjects::publish_subject<pw::node> _mediaAddedSubject;
    rpp::subjects::publish_subject<pw::node> _mediaRemovedSubject;
};
