//
// Created by Tudor Coman on 02.04.2022.
//

#include "../headers/WebService.h"

WebService::WebService() :
    angajatController(AngajatController("/angajat")),
    produsController(ProdusController("/produs")),
    depozitController(DepozitController("/depozit", angajatController.angajatRepository)),
    tranzactieController("/tranzactie", depozitController.depoziteRepository, produsController.produsRepository),
    requestHandler(_create_request_handler()),
    listener(HTTPListener("http://localhost:8081", "/api", requestHandler)) { }

void WebService::run() {
    listener.open()
            .then([](){printf("\nStarting server on port 8081\n");})
            .wait();
    while(true);
}

HTTPListener::RequestHandler WebService::_create_request_handler() {
    return HTTPListener::RequestHandler()
            .support(angajatController.getRequestHandler())
            .support(produsController.getRequestHandler())
            .support(depozitController.getRequestHandler())
            .support(tranzactieController.getRequestHandler());
}
