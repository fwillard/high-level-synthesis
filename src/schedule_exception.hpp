//
//  ScheduleException.hpp
//  hlsyn
//
//  Created by Finn Willard on 12/3/20.
//

#ifndef ScheduleException_hpp
#define ScheduleException_hpp

#include <string>
#include <exception>

class ScheduleException: std::exception {
private:
    std::string message_;
public:
    explicit ScheduleException(const std::string& message);
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

#endif /* ScheduleException_hpp */
