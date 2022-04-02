//
// Created by Tudor Coman on 02.04.2022.
//

#ifndef OOP_WEBSERVICE_H
#define OOP_WEBSERVICE_H

#include "AngajatController.h"

class WebService {
    AngajatController angajatController;
    HTTPListener::RequestHandler requestHandler;
    HTTPListener listener;

    HTTPListener::RequestHandler _create_request_handler();
public:
    WebService();

    void run();
};

#endif //OOP_WEBSERVICE_H