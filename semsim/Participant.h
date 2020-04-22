//
// Created by Ciaran on 4/20/2020.
//

#ifndef LIBSEMGEN_PARTICIPANT_H
#define LIBSEMGEN_PARTICIPANT_H

#include "librdf.h"
#include <string>
#include "semsim/Resource.h"
#include "Subject.h"
#include "Triple.h"


namespace semsim {

    /*
     * Superclass of participant types allow us
     * to group participant types into vector.
     */
    class Participant {
        librdf_world* world_;
        std::string subject_;
        Resource resource_;
        PredicatePtr predicate_ptr_;
        double multiplier_;
        std::string physicalEntityReference_;
    public:

        Participant(librdf_world *world, std::string subject, PredicatePtr predicate, Resource resource, double multiplier,
                    std::string physicalEntityReference);

        std::vector<Triple> toTriples();

        PredicatePtr getPredicatePtr();

        void setPredicatePtr(PredicatePtr predicate_ptr);

        librdf_world *getWorld() const;

        const std::string &getSubject() const;

        const Resource &getResource() const;

        double getMultiplier() const;

        const std::string &getPhysicalEntityReference() const;
    };



    typedef std::shared_ptr<Participant> ParticipantPtr;



    class ParticipantSource : public Participant {

    public:
        ParticipantSource(librdf_world *world, std::string subject, Resource resource,
                          double multiplier,
                          std::string physicalEntityReference);
    };


    class ParticipantSink : public Participant {
    private:

        double multiplier_;
        std::string physicalEntityReference_;
    public:

    ParticipantSink(
            librdf_world *world, std::string subject, Resource resource,
            double multiplier, std::string physicalEntityReference);

    };


    class ParticipantMediator : public Participant {
    private:
        double multiplier_ = 0.0;

    public:

    ParticipantMediator(
            librdf_world *world, std::string subject, Resource resource,
            std::string physicalEntityReference);

    };

}


#endif //LIBSEMGEN_PARTICIPANT_H