/***************************************************************************
 *   Copyright (C) 2005 by Robot Group Leipzig                             *
 *    martius@informatik.uni-leipzig.de                                    *
 *    fhesse@informatik.uni-leipzig.de                                     *
 *    der@informatik.uni-leipzig.de                                        *
 *                                                                         *
 *   ANY COMMERCIAL USE FORBIDDEN!                                         *
 *   LICENSE:                                                              *
 *   This work is licensed under the Creative Commons                      *
 *   Attribution-NonCommercial-ShareAlike 2.5 License. To view a copy of   *
 *   this license, visit http://creativecommons.org/licenses/by-nc-sa/2.5/ *
 *   or send a letter to Creative Commons, 543 Howard Street, 5th Floor,   *
 *   San Francisco, California, 94105, USA.                                *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.                  *
 *                                            * 
 *                                                                         *
 *   $Log$
 *   Revision 1.2  2007-12-07 09:13:46  fhesse
 *   adapted paths due to moving of controller files
 *   to simulation/hand directory
 *
 *   Revision 1.1  2007/12/07 09:05:30  fhesse
 *   moved controller files specialized for hand here
 *   from selforg/controller directory
 *
 *   Revision 1.2  2007/09/23 15:33:06  fhesse
 *   testing controller ...
 *
 *   Revision 1.1  2007/09/21 16:26:08  fhesse
 *   initial version (for testing only!)
 *
 *   Revision 1.1  2007/09/18 13:02:12  fhesse
 *   initial version of hebb-invertnchannelcontroller for hand (6 motors, 5 irsensors)
 *
 *   Revision 1.1  2007/09/17 19:25:47  fhesse
 *   initial version
 *
 *                                                                         *
 ***************************************************************************/
#ifndef __INVERTNCHANNELCONTROLLERHEBBHHAND_H
#define __INVERTNCHANNELCONTROLLERHEBBHHAND_H

#include "selforg/invertnchannelcontroller.h"

#include <assert.h>
#include <math.h>
#include <math.h>
#include <vector.h>

#include <selforg/matrix.h>



/**
 * class for robot controller that uses the georg's matrixlib for 
 *  direct matrix inversion for n channels 
 * (simple one layer networks)
 * 
 * Implements standart parameters: eps, rho, mu, stepnumber4avg, stepnumber4delay
 */
class InvertNChannelControllerHebbHHand : public InvertNChannelController {

public:
  InvertNChannelControllerHebbHHand(int _buffersize, bool _update_only_1=false, bool inactivate_hebb=false);
  virtual void init(int sensornumber, int motornumber);

  virtual ~InvertNChannelControllerHebbHHand(){}

  /// returns the number of sensors the controller was initialised with or 0 if not initialised
  virtual int getSensorNumber() const { return number_all_sensors; }


  /// performs one step (includes learning). 
  /// Calulates motor commands from sensor inputs.
  virtual void step(const sensor* , int number_sensors, motor* , int number_motors);


  /// performs one step without learning. Calulates motor commands from sensor inputs.
  virtual void stepNoLearning(const sensor* , int number_sensors, 
			      motor* , int number_motors);


  // inspectable interface
  virtual std::list<iparamkey> getInternalParamNames() const;
  virtual std::list<iparamval> getInternalParams() const;



  virtual paramval getParam(const paramkey& key) const{
    if(key == "eps_hebb") return eps_hebb; 
    else if(key == "fact_eps_h") return fact_eps_h;     
    else  return InvertNChannelController::getParam(key) ;
  }

  virtual bool setParam(const paramkey& key, paramval val){
    if(key == "eps_hebb") eps_hebb=val;
    else if(key == "fact_eps_h") fact_eps_h=val;
    else  return InvertNChannelController::setParam(key,val) ;
    return true;
  }

  virtual paramlist getParamList() const{
    paramlist list = InvertNChannelController::getParamList();
    list.push_back(std::pair<paramkey, paramval> ("eps_hebb", eps_hebb));
    list.push_back(std::pair<paramkey, paramval> ("fact_eps_h", fact_eps_h));
    return list;
  }


protected:
  paramval eps_hebb;
  paramval fact_eps_h;
  unsigned int number_motors; // number of motors used

  bool hebb_inactive; //if true: deactivates hebb learning part and the context sensors, so it is the normal homeokinetic controller (invertnchannelcontroller)

  unsigned short number_all_sensors;
  matrix::Matrix xsi_org;  // modeling error homeokinese 
  matrix::Matrix xsi_hebb; // modeling error hebb 
  sensor* all_sensors; //memory for all sensor values (used for hebbian learning)
  std::vector <sensor>  context_sensors; //memory for context sensor values 
  matrix::Matrix p; //weights for hebbian learning
  matrix::Matrix sensors_mean; // mean of sensors for hebb learning

  //double p0_pos, p0_neg, p1_pos, p1_neg;

  sensor old_sensors[10]; //memory of old sensor values (used for hebbian learning)

  //virtual matrix::Matrix hebb(matrix::Matrix& xsi, sensor* sensors);
  virtual matrix::Matrix hebbh(matrix::Matrix& xsi, std::vector<sensor>* c_sensors, std::vector<int>* H_reset);
  //virtual double calculateEHebb(const matrix::Matrix& x_delay, const matrix::Matrix& y_delay);      
  virtual void learn(const matrix::Matrix& x_delay, const matrix::Matrix& y_delay);

//  std::vector<bool> IRsensed;
//  std::vector<bool> IRsensed_old;

  std::vector<int> IRused;
  std::vector<int> IRused_old;
  int number_hebb_sensors;

};

#endif
