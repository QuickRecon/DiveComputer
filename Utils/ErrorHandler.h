//
// Created by aren on 28/10/21.
//

#ifndef DIVECOMPUTER_ERRORHANDLER_H
#define DIVECOMPUTER_ERRORHANDLER_H


class ErrorHandler {
public:
    ErrorHandler();

    void Error(char *message);
};

ErrorHandler errorHandler = ErrorHandler();
#endif //DIVECOMPUTER_ERRORHANDLER_H
