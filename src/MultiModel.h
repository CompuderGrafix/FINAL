/*
 * MultiModel.h
 *
 *  Created on: Dec 8, 2012
 *      Author: soujiroboi
 */

#ifndef MULTIMODEL_H_
#define MULTIMODEL_H_

#include "Model.h"

class MultiModel {
  private:
    Model* model;
    mat4 offset;
    mat4 movement;
    std::vector<MultiModel> children;

  public:
    MultiModel();
    virtual ~MultiModel();

    void draw();
};

#endif /* MULTIMODEL_H_ */
