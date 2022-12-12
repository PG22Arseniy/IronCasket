// Black Board Keys

#pragma once

#include "Runtime/Core/Public/UObject/NameTypes.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"

namespace bb_keys
{
	TCHAR const * const TargetLocation = TEXT ("TargetLocation");
	TCHAR const * const PlayerDetected = TEXT ("PlayerDetected");
	TCHAR const * const PlayerInRange = TEXT ("PlayerInRange");  
	TCHAR const * const PlayerWithinMinimumDistance = TEXT ("PlayerWithinMinimumDistance");   
	TCHAR const * const InShootingAngle = TEXT ("InShootingAngle");    
	TCHAR const * const UnableToShoot = TEXT ("UnableToShoot");     
	TCHAR const * const PlayerMech = TEXT ("PlayerMech");      
	TCHAR const * const CanSeePlayer = TEXT ("CanSeePlayer");       
	TCHAR const * const CloseToItemInCircle = TEXT ("CloseToItemInCircle");        
	TCHAR const * const PositionInCircle = TEXT ("PositionInCircle");         
	TCHAR const * const GoingBack = TEXT ("GoingBack");          
	TCHAR const * const Hide = TEXT ("Hide");           
	TCHAR const * const UnderAttack = TEXT ("UnderAttack");            
	TCHAR const * const DesiredLocation = TEXT ("DesiredLocation");            
	TCHAR const * const CanGenerateCircle = TEXT ("CanGenerateCircle");             
}
