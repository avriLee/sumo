#ifndef MSUPDATEEACHTIMESTEPCONTAINER_H
#define MSUPDATEEACHTIMESTEPCONTAINER_H

/**
 * @file   MSUpdateEachTimestepContainer.h
 * @author Christian Roessel
 * @date   Started Thu Oct 23 16:38:35 2003
 * @version $Id$
 * @brief  
 * 
 * 
 */

/* Copyright (C) 2003 by German Aerospace Center (http://www.dlr.de) */

//---------------------------------------------------------------------------//
//
//   This program is free software; you can redistribute it and/or modify
//   it under the terms of the GNU General Public License as published by
//   the Free Software Foundation; either version 2 of the License, or
//   (at your option) any later version.
//
//---------------------------------------------------------------------------//

#include <vector>

template< class UpdateEachTimestep >
class MSUpdateEachTimestepContainer
{
public:    
    static MSUpdateEachTimestepContainer* getInstance( void )
        {
            if ( instanceM == 0 ){
                instanceM = new MSUpdateEachTimestepContainer();
            }
            return instanceM;
        }

    void addItemToUpdate( UpdateEachTimestep* item )
        {
            containerM.push_back( item );
        }

    void updateAll( void )
        {
            for_each( containerM.begin(), containerM.end(),
                      mem_fun( &UpdateEachTimestep::updateEachTimestep ) );
        }

    ~MSUpdateEachTimestepContainer( void )
        {
            containerM.clear();
            instanceM = 0;
        }
    
private:
    MSUpdateEachTimestepContainer( void )
        : containerM()
        {}

    std::vector< UpdateEachTimestep* > containerM;
    
    static MSUpdateEachTimestepContainer* instanceM;
};

// initialize static member
template< class UpdateEachTimestep >
MSUpdateEachTimestepContainer< UpdateEachTimestep >*
MSUpdateEachTimestepContainer< UpdateEachTimestep >::instanceM = 0;


#endif // MSUPDATEEACHTIMESTEPCONTAINER_H

// Local Variables:
// mode:C++
// End:
