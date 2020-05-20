//
// Created by Ciaran on 4/14/2020.
//


#include "semsim/MetaID.h"

semsim::MetaID::MetaID(std::string base, long number, int num_digits) :
        base_(std::move(base)),
        number_(number),
        num_digits_(num_digits) {

    // check that we're not asked to do the impossible
    if (number_ > maxNumber()) {
        std::ostringstream err_stream;
        err_stream << "The maximum number of IDs you can have with " << num_digits_ <<
                   " is " << maxNumber() << ". Please increase the num_digits_ argument." << std::endl;
        throw std::invalid_argument(err_stream.str());
    }
}

bool semsim::MetaID::operator==(const semsim::MetaID &rhs) const {
    return this->generate().c_str() == rhs.generate().c_str();
}

bool semsim::MetaID::operator!=(const semsim::MetaID &rhs) const {
    return !(rhs == *this);
}

int semsim::MetaID::maxNumber() const {
    std::ostringstream os;
    for (int i = 0; i < num_digits_; i++) {
        os << "9";
    }
    return std::stoi(os.str());
}

int semsim::MetaID::countDigits(long n) {
    if (n == 0) {
        return 1;
    }
    int count = 0;
    while (n != 0) {
        n = n / 10;
        ++count;
    }
    return count;
}

std::string semsim::MetaID::generate() const {
    return generate(number_);
}

std::string semsim::MetaID::generate(long n) const {
    // work out how many 0's we need to pad the ID
    int number_of_digits_in_number = countDigits(n);
    if (number_of_digits_in_number > num_digits_) {
        throw std::logic_error("This should never happen as it was caught in the "
                               "last check");
    }
    int padding_needed = num_digits_ - number_of_digits_in_number;

    std::ostringstream os;
    os << base_;
    for (int i = 0; i < padding_needed; i++) {
        os << "0";
    }
    os << n;
    return os.str();
}

