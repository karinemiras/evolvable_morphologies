//
//  GenomeTransceiver.h
//  RobotController
//
//  Created by Berend Weel on 10/04/15.
//  Copyright (c) 2015 Berend Weel. All rights reserved.
//

#ifndef __RobotController__GenomeTransceiver__
#define __RobotController__GenomeTransceiver__

class GenomeTransceiver {
public:
    virtual void broadcastGenome() = 0;
    
    virtual void receive() = 0;
    
    virtual void step() = 0;
};

#endif /* defined(__RobotController__GenomeTransceiver__) */
