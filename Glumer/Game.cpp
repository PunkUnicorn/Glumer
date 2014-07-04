#include "amazingfx.h"

#include "./OffInternet/glQuaternion.h"
#include "MeteorToolkit.h"
#include "cTimerWrapper.h"
#include "cObjectMoveable.h"
#include "cGameShapeCollisionEngine.h"
#include "cGameShapeFactory.h"
#include "cGameShape_SwitchGadget.h"

#include <random>
#include ".\OffInternet\ReadWriteLock.h"
#include "cSelectableBase.h"

static std::vector<cObjectMoveable::PTR> my_showlist;
static CReadWriteLock showlistLock;
static void ShowistAdd(cObjectMoveable::PTR addme)
{
	showlistLock.LockWriter();
		my_showlist.push_back(addme);
	showlistLock.UnlockWriter();
}

static cGameShapeCollisionEngine engine1;
static cGameShapeCollisionEngine engine2;

static cGameShapeFactory shape_factory;
static cHUD_Colour *unversal_hud_colour;
static 	cGameShape_HUDShortRadar::PTR my_radar1;
static cGameShape_HUDLongRadar::PTR my_radar2;

static inline void cGameShape_ApplyCollisionVelocity(float &velocity1, float mass1, float &velocity2, float mass2)
{
	float result1;
	result1 = velocity1 * (mass1 - mass2);
	result1 += 2.0f * mass2*velocity2;
	result1 /= mass1+mass2;

	float result2;
	result2 = velocity2 * (mass2 - mass1);
	result2 += 2.0f * mass1*velocity1;
	result2 /= mass1+mass2;

	velocity1 = result1;
	velocity2 = result2;
	return;
}
static /*inline*/ void cGameShape_ReCalcVelocity(cDirection &recalc)
{
	recalc.FactorySetVelocity(max(recalc.GetX(), recalc.GetY()));
	recalc.FactorySetVelocity(max(recalc.GetVelocity(), recalc.GetZ()));
}

static inline void ApplyMeteorCollision(cGameShapeCollisionBase *center, cGameShapeCollisionBase *otherone)
{
	float x, y, z;
	x = center->GetDirection().GetX();
	y = center->GetDirection().GetY();
	z = center->GetDirection().GetZ();

	float ox, oy, oz;
	ox = otherone->GetDirection().GetX();
	oy = otherone->GetDirection().GetY();
	oz = otherone->GetDirection().GetZ();

	cGameShape_ApplyCollisionVelocity(x, center->GetRadius(), ox, otherone->GetRadius());
	cGameShape_ApplyCollisionVelocity(y, center->GetRadius(), oy, otherone->GetRadius());
	cGameShape_ApplyCollisionVelocity(z, center->GetRadius(), oz, otherone->GetRadius());

	center->GetDirectionWrite().FactorySetX(x);
	center->GetDirectionWrite().FactorySetY(y);
	center->GetDirectionWrite().FactorySetZ(z);

	otherone->GetDirectionWrite().FactorySetX(ox);
	otherone->GetDirectionWrite().FactorySetY(oy);
	otherone->GetDirectionWrite().FactorySetZ(oz);


	//cGameShape_ApplyCollisionVelocity(center->GetDirection().GetX(), center->GetRadius(), otherone->GetDirection().GetX(), otherone->GetRadius());
	//cGameShape_ApplyCollisionVelocity(center->GetDirection().GetY(), center->GetRadius(), otherone->GetDirection().GetY(), otherone->GetRadius());
	//cGameShape_ApplyCollisionVelocity(center->GetDirection().GetZ(), center->GetRadius(), otherone->GetDirection().GetZ(), otherone->GetRadius());

	cGameShape_ReCalcVelocity(center->GetDirectionWrite());
	cGameShape_ReCalcVelocity(otherone->GetDirectionWrite());
}
static inline void CrashMeteors(cGameShapeCollisionBase::TRILATERATION &crash)
{
	/*std::vector<cGameShape_Meteor::PTR>::iterator first, second;
	first = cGameShapeFactory_PerShape<cGameShape_Meteor>::Get(crash.mCenterID);
	second = cGameShapeFactory_PerShape<cGameShape_Meteor>::Get(crash.mDistanceUnits[0].id);*/
	cGameShapeCollisionBase::PTR first, second;
	first = cGameShapeFactory::Get(crash.mCenterID);
	second = cGameShapeFactory::Get(crash.mDistanceUnits[0].id);

	if (first.ptr->mIgnoreCollisions)
		return;
	if (second.ptr->mIgnoreCollisions)
		return;

	static char info_line[80];				
	static int bangCount = 0;
	sprintf_s(info_line, "BANG %i", bangCount++);
	cPolyLetter_Console::debug_console->AddMessage(info_line);

	ApplyMeteorCollision(first.ptr, second.ptr);
}
static void CollisionProcessing(std::vector<cGameShapeCollisionBase::TRILATERATION> *collisionResults)
{
	// radar collisions for ping results
	////my_radar2.ptr->mLongRadar_Data.mCollisionResults.clear();

	////my_radar2.ptr->mLongRadar.SetInclusionTest(&my_radar2.ptr->mLongRadar_Data);
	////		
	////my_radar2.ptr->mRadarCenter.mCenter.mX = shape_factory.mWorldOffset.mCenter.mX;
	////my_radar2.ptr->mRadarCenter.mCenter.mY = shape_factory.mWorldOffset.mCenter.mY;
	////my_radar2.ptr->mRadarCenter.mCenter.mZ = shape_factory.mWorldOffset.mCenter.mZ;
	////my_radar2.ptr->mRadarCenter.mRadius = my_radar2.ptr->mRange;
	////my_radar2.ptr->mLongRadar.FindCollisions(&my_radar2.ptr->mRadarCenter);

	/////*THIEF!!!! ---->*/ my_radar1.ptr->mBorrowedCollisionData = &(my_radar2.ptr->mLongRadar_Data);


	//const cGameShapeCollisionBase::PTR pimp = { &cGameShapeFactory::mWorldOffset };
	std::vector<cGameShapeCollisionBase::TRILATERATION> *results = collisionResults;//cGameShapeCollisionEngine::GetCollisionResults(pimp, pimp.ptr->mRadius);
	//cGameShapeCollisionBase::mCollisionReadWriteLock.LockWriter();
		std::for_each(results->begin(), results->end(), CrashMeteors);
	//cGameShapeCollisionBase::mCollisionReadWriteLock.UnlockWriter();
}

static inline void CheckGameShapeBounds(cGameShapeBase *shape)
{
	const float WORLD_RADIUS = 1000.0f;
	const float WORLD_DIAMITER = 2000.0f;

#define	CAMERA_RELATIVE_COORD(X) shape->GetCenter().##X+cGameShapeFactory::mWorldOffset.GetCenter().##X

	if (CAMERA_RELATIVE_COORD(GetX()) > WORLD_RADIUS )
		shape->GetCenterWrite().AddToX(-WORLD_DIAMITER);
	else if (CAMERA_RELATIVE_COORD(GetX()) < -WORLD_RADIUS )
		shape->GetCenterWrite().AddToX(WORLD_DIAMITER);

	if (CAMERA_RELATIVE_COORD(GetY()) > WORLD_RADIUS )
		shape->GetCenterWrite().AddToY(-WORLD_DIAMITER);
	else if (CAMERA_RELATIVE_COORD(GetY()) < -WORLD_RADIUS )
		shape->GetCenterWrite().AddToY(WORLD_DIAMITER);

	if (CAMERA_RELATIVE_COORD(GetZ()) > WORLD_RADIUS )
		shape->GetCenterWrite().AddToZ(-WORLD_DIAMITER);

	else if (CAMERA_RELATIVE_COORD(GetZ()) < -WORLD_RADIUS)
		shape->GetCenterWrite().AddToZ(WORLD_DIAMITER);

#undef CAMERA_RELATIVE_COORD
}

static inline void CheckMeteorBound(cGameShape_Meteor::PTR &meteor)
{
	CheckGameShapeBounds(meteor.ptr);
}
//static inline void CheckRadarBound(cGameShape_HUDShortRadar::PTR &radar)
//{
//	//CheckGameShapeBounds(&radar.ptr->mRadarCenter);
//}

static void GameWorldBoundsProcessing(void)
{
	std::for_each(cGameShapeFactory::mMeteor.mList.begin(), cGameShapeFactory::mMeteor.mList.end(), CheckMeteorBound);
	//std::for_each(cGameShapeFactory::mRadar.mList.begin(), cGameShapeFactory::mRadar.mList.end(), CheckRadarBound);
}

static char* HappyMessage(void)
{
	static char *msg[] = { { "Happy birthday"}, {"A"}, {"B"} }; 
	static int i = 0;
	if (i == 3)
	{
		i = 0;
	}
	return msg[i++];
}
static void DebugMessageHappy(cGameShape_SwitchGadget *sender)
{
	static char info_line[80];
	sprintf_s(info_line, "CLK %i %s", sender->mSelected, HappyMessage());
	cPolyLetter_Console::debug_console->AddMessage(info_line);
}
static void ShortRangeRadarOnOff(cGameShape_SwitchGadget *sender)
{
	static bool off = false;
	off = !off;
	my_radar1.ptr->SetVisible(off);
}
static float GetRandomish()
{
	const static float one[] = {2.0f, 3.5f, 1.0f, 0.5f, 3.0f};
	const static float two[] = {1.0f, 3.3f, 2.7f, 3.0f, 3.1f};
	const static float three[] = {0.4f, 0.2f, 1.3f, 2.3f, 0.6f};
	static int index = 0;
	if (index == 5)
		index = 0;

	const static float *all[] = {one, two, three};
	static int listIndex = 0;
	if (listIndex == 3)
		listIndex = 0;

	const static float modifier[] = {-0.1f, 0.2f, -0.3f, 0.4f, -0.5f, 0.6f, -0.7f};
	static int modIndex = 6;	
	if (modIndex == -1)
		modIndex = 6;

	return (all[listIndex++][index++]) * (modifier[modIndex--]);
}
static void CreateNewMeteor(cGameShape_SwitchGadget *sender)
{
	cGameShape_Meteor::PTR my_shape;
    cObjectMoveable::PTR my_obj;

//#define RANDOMF  ((distribution(generator)-1000) / 10.0f)
#define RANDOMF  (GetRandomish())
#define POSRANDOMF (abs(RANDOMF))
	
	if (cGameShapeCollisionBase::mCollisionReadWriteLock.WriteLockCount + cGameShapeCollisionBase::mCollisionReadWriteLock.ReadLockCount > 0)
	{
		return;
	}
	float scale = POSRANDOMF*10.0f;
	bool trueFalse = false;
	for (int yy=0; yy<10; yy++)
	{
		trueFalse = !trueFalse;
		scale += (yy/2);

		my_shape = shape_factory.NewMeteor(unversal_hud_colour, scale, false);
		my_obj.ptr = my_shape.ptr;
		ShowistAdd(my_obj);
		cGameShapeCollisionBase::PTR gscb_pimp = { my_shape.ptr };
		
		if (trueFalse)
			engine1.GetWorkList().push_back(gscb_pimp);
		else
			engine2.GetWorkList().push_back(gscb_pimp);

		my_shape.ptr->GetCenterWrite().FactorySetX((RANDOMF/5.0f)-RANDOMF*300);
		my_shape.ptr->GetCenterWrite().FactorySetY((RANDOMF/5.0f)-RANDOMF*300);
		my_shape.ptr->GetCenterWrite().FactorySetZ((RANDOMF/5.0f)-RANDOMF*300);

		my_shape.ptr->mOrientation.FactorySetAngleX(RANDOMF);
		my_shape.ptr->mOrientation.FactorySetAngleY(RANDOMF);
		my_shape.ptr->mOrientation.FactorySetAngleZ(RANDOMF);
		my_shape.ptr->mOrientation.FactorySetAngleIncrement(RANDOMF);

		my_shape.ptr->GetDirectionWrite().FactorySetX((RANDOMF/5.0f)-RANDOMF);
		my_shape.ptr->GetDirectionWrite().FactorySetY((RANDOMF/5.0f)-RANDOMF);
		my_shape.ptr->GetDirectionWrite().FactorySetZ((RANDOMF/5.0f)-RANDOMF);
		my_shape.ptr->GetDirectionWrite().FactorySetVelocity(RANDOMF);

		my_shape.ptr->Start(scale, &cGameShapeFactory::mWorldOffset);
	}
#undef POSRANDOMF
#undef RANDOMF
}

static void MakeBullshitShapes(void)
{
	cGameShape_Meteor::PTR my_shape;
    cObjectMoveable::PTR my_obj;

    unversal_hud_colour = new cHUD_Colour();
    
    cPolyLetter_Console::debug_console = new cPolyLetter_Console(unversal_hud_colour, 0.5f);

	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 10.0f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);


	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine1.GetWorkList().push_back(pimp);
	}*/
	
	my_shape.ptr->GetCenterWrite().FactorySetX(0.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(0.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(-80.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_shape.ptr->mOrientation.FactorySetAngleY(0.9f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(0.8f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(1.25f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.05f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 1.0f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);


	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine2.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(16.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(15.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(-19.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.25f);
	my_shape.ptr->mOrientation.FactorySetAngleY(0.75f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(1.0f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(0.25f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.002f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0005f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.001f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 1.0f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);

	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine2.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(-4.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(-3.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(-30.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.05f);
	my_shape.ptr->mOrientation.FactorySetAngleY(1.0f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(4.0f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(0.5f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.005f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 2.0f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);

	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine2.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(-8.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(0.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(415.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.05f);
	my_shape.ptr->mOrientation.FactorySetAngleY(1.0f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(16.0f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(0.05f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.02f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 0.5f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);

	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine2.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(10.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(0.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(415.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.75f);
	my_shape.ptr->mOrientation.FactorySetAngleY(1.0f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(0.5f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(2.0f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.005f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 20.0f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);

	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine1.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(-200.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(0.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(400.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.1f);
	my_shape.ptr->mOrientation.FactorySetAngleY(0.2f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(0.3f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(1.5f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.1f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	my_shape = shape_factory.NewMeteor(unversal_hud_colour, 12.3f);
	my_obj.ptr = my_shape.ptr;
	ShowistAdd(my_obj);

	/*{
		cGameShapeCollisionBase::PTR pimp = { my_shape.ptr };
		engine1.GetWorkList().push_back(pimp);
	}*/

	my_shape.ptr->GetCenterWrite().FactorySetX(200.0f);
	my_shape.ptr->GetCenterWrite().FactorySetY(200.0f);
	my_shape.ptr->GetCenterWrite().FactorySetZ(400.0f);

	my_shape.ptr->mOrientation.FactorySetAngleX(0.0f);
	my_shape.ptr->mOrientation.FactorySetAngleY(0.1f);
	my_shape.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_shape.ptr->mOrientation.FactorySetAngleIncrement(5.0f);

	my_shape.ptr->GetDirectionWrite().FactorySetX(0.025f);
	my_shape.ptr->GetDirectionWrite().FactorySetY(0.025f);
	my_shape.ptr->GetDirectionWrite().FactorySetZ(0.025f);


	//SwitchGadgets
	cGameShape_SwitchGadget::PTR my_gadget1;
	my_gadget1 = shape_factory.NewSwitchGadget(unversal_hud_colour, 0.1f, &cGameShape_SwitchGadget::PistonShift_OnOffPulse, &DebugMessageHappy);
	my_obj.ptr = my_gadget1.ptr;
	ShowistAdd(my_obj);

	my_gadget1.ptr->GetCenterWrite().FactorySetX(-1.0f);
	my_gadget1.ptr->GetCenterWrite().FactorySetY(1.0f);
	my_gadget1.ptr->GetCenterWrite().FactorySetZ(-4.5f);

	my_gadget1.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_gadget1.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_gadget1.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_gadget1.ptr->mOrientation.FactorySetAngle(25.0f);
	my_gadget1.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_gadget1.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_gadget1.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_gadget1.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	cGameShape_SwitchGadget::PTR my_gadget2;
	my_gadget2 = shape_factory.NewSwitchGadget(unversal_hud_colour, 0.1f, &cGameShape_SwitchGadget::PistonShift_OnOffClick, ShortRangeRadarOnOff);
	my_obj.ptr = my_gadget2.ptr;
	ShowistAdd(my_obj);

	my_gadget2.ptr->GetCenterWrite().FactorySetX(-1.0f);
	my_gadget2.ptr->GetCenterWrite().FactorySetY(0.5f);
	my_gadget2.ptr->GetCenterWrite().FactorySetZ(-4.2f);

	my_gadget2.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_gadget2.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_gadget2.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_gadget2.ptr->mOrientation.FactorySetAngle(25.0f);
	my_gadget2.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_gadget2.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_gadget2.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_gadget2.ptr->GetDirectionWrite().FactorySetZ(0.0f);

	cGameShape_SwitchGadget::PTR my_gadget3;
	my_gadget3 = shape_factory.NewSwitchGadget(unversal_hud_colour, 0.1f, &cGameShape_SwitchGadget::PistonShift_ButtonClick, &CreateNewMeteor);
	my_obj.ptr = my_gadget3.ptr;
	ShowistAdd(my_obj);

	my_gadget3.ptr->GetCenterWrite().FactorySetX(-1.0f);
	my_gadget3.ptr->GetCenterWrite().FactorySetY(0.0f);
	my_gadget3.ptr->GetCenterWrite().FactorySetZ(-3.9f);

	my_gadget3.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_gadget3.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_gadget3.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_gadget3.ptr->mOrientation.FactorySetAngle(25.0f);
	my_gadget3.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_gadget3.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_gadget3.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_gadget3.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	cGameShape_SwitchGadget::PTR my_gadget4;
	my_gadget4 = shape_factory.NewSwitchGadget(unversal_hud_colour, 0.1f, &cGameShape_SwitchGadget::PistonShift_ButtonDoesntKnowWhatItIs, NULL);
	my_obj.ptr = my_gadget4.ptr;
	ShowistAdd(my_obj);

	my_gadget4.ptr->GetCenterWrite().FactorySetX(-1.0f);
	my_gadget4.ptr->GetCenterWrite().FactorySetY(-0.5f);
	my_gadget4.ptr->GetCenterWrite().FactorySetZ(-3.6f);

	my_gadget4.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_gadget4.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_gadget4.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_gadget4.ptr->mOrientation.FactorySetAngle(25.0f);
	my_gadget4.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_gadget4.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_gadget4.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_gadget4.ptr->GetDirectionWrite().FactorySetZ(0.0f);


	//radars
	//cGameShape_HUDShortRadar::PTR my_radar1;
	my_radar1 = shape_factory.NewHUDShortRadar(unversal_hud_colour, 500.0f, 0.005f);
	my_obj.ptr = my_radar1.ptr;
	ShowistAdd(my_obj);

	my_radar1.ptr->GetCenterWrite().FactorySetX(0.0f);
	my_radar1.ptr->GetCenterWrite().FactorySetY(0.5f);
	my_radar1.ptr->GetCenterWrite().FactorySetZ(-8.4f);

	my_radar1.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_radar1.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_radar1.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_radar1.ptr->mOrientation.FactorySetAngle(25.0f);
	my_radar1.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_radar1.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_radar1.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_radar1.ptr->GetDirectionWrite().FactorySetZ(0.0f);



	//cGameShape_HUDLongRadar::PTR my_radar2;
	my_radar2 = shape_factory.NewHUDLongRadar(unversal_hud_colour, 1000.0f, 0.005f);
	my_obj.ptr = my_radar2.ptr;
	//ShowistAdd(my_obj);

	my_radar2.ptr->GetCenterWrite().FactorySetX(0.0f);
	my_radar2.ptr->GetCenterWrite().FactorySetY(0.5f);
	my_radar2.ptr->GetCenterWrite().FactorySetZ(-8.4f);

	my_radar2.ptr->mOrientation.FactorySetAngleX(1.0f);
	my_radar2.ptr->mOrientation.FactorySetAngleY(0.0f);
	my_radar2.ptr->mOrientation.FactorySetAngleZ(0.0f);
	my_radar2.ptr->mOrientation.FactorySetAngle(25.0f);
	my_radar2.ptr->mOrientation.FactorySetAngleIncrement(0.0f);

	my_radar2.ptr->GetDirectionWrite().FactorySetX(0.0f);
	my_radar2.ptr->GetDirectionWrite().FactorySetY(0.0f);
	my_radar2.ptr->GetDirectionWrite().FactorySetZ(0.0f);
}
DllDeclspec void DrawScene_Init(void)
{
	MakeBullshitShapes();

	// start two collision engines to share half collision testing each but stagger them to run alternatively. They share the otherwise 
	// big spike if processing one big massive list
	const unsigned int alternateSlotLikePistonsDelay = cGameShapeCollisionEngine::DUAL_ENGINE_CALLBACKDELAY / 2;
	engine1.Start(cGameShapeCollisionBase::mCollisionList, &CollisionProcessing);
	SDL_Delay(alternateSlotLikePistonsDelay);
	engine2.Start(std::vector<cGameShapeCollisionBase::PTR>(), &CollisionProcessing);

	shape_factory.Start();


	/*StarWarsText bandText;
	bandText.mText.AddMessage("Traveling constantly through space 'n time in search of ");
	bandText.mText.AddMessage("Gomez and snacks, Frodo Freud have travel in some of the more ");
	bandText.mText.AddMessage("unlikely places, like Bracknell, London and more. On their ");
	bandText.mText.AddMessage("self-appointed journey through space and time healing all the things");
	bandText.mText.AddMessage("that went wrong, they find themselves stealing a space ship... but from where???");

	cMovement_Center starWarsScroll;
	starWarsScroll.FactorySetX(0.0f);
	starWarsScroll.FactorySetY(0.0f);
	starWarsScroll.FactorySetZ(-5.0f);*/
}
DllDeclspec void DrawScene_Close(void)
{

}

DllDeclspec void DrawScene(int r, int g, int b)
{
	static unsigned int time = 0;

    unversal_hud_colour->m_red = r;
    unversal_hud_colour->m_green = g;
    unversal_hud_colour->m_blue = b;

	showlistLock.LockReader();
		std::for_each(my_showlist.begin(), my_showlist.end(), cObjectMoveable::FireEventShow);
	showlistLock.UnlockReader();

    glLoadIdentity();
    glTranslatef(2.0f, 4.0f, -20.0f);
    cPolyLetter_Console::debug_console->EventShow();
	 glLoadIdentity();
}

DllDeclspec void GameGuts(void)
{
	GameWorldBoundsProcessing();
}

DllDeclspec void PointerHitTest(unsigned int mouse_x, unsigned int mouse_y, int mouse_z)
{
	cSelectableBase::EventClick(&cGameShapeFactory::mSelectableDrawList, mouse_x, mouse_y, mouse_z);
}

DllDeclspec void PointerMotionChange(unsigned int mouse_x, unsigned int mouse_y, int mouse_z)
{
	int x = mouse_x - 512;
	int y = mouse_y - 384;
}

const static float piover180 = 0.0174532925f;

DllDeclspec void FirstPersonStrafeRight(float step)
{
	shape_factory.mWorldOffset.GetDirectionWrite().AddToX(0.2f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
}
DllDeclspec void FirstPersonStrafeLeft(float step)
{
	shape_factory.mWorldOffset.GetDirectionWrite().AddToX(-0.2f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
}
DllDeclspec void FirstPersonForwardMove(float step)
{	
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetX(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().AddToZ(-0.2f);
}

DllDeclspec void FirstPersonBackMove(float step)
{	
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetX(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().AddToZ(0.2f);
}
DllDeclspec void FirstPersonDownMove(float step)
{
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetX(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().AddToY(-0.2f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
	return;
}
DllDeclspec void FirstPersonUpMove(float step)
{	
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetX(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().AddToY(0.2f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
	return;
}
DllDeclspec void FirstPersonStop(void)
{
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetX(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
	return;
}
DllDeclspec void FirstPersonAngleChange(float heading_angle_x, float heading_angle_y, float forward_unit = 0.0f)
{
	shape_factory.mWorldOffset.GetDirectionWrite().AddToX(heading_angle_x);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetY(0.0f);
	shape_factory.mWorldOffset.GetDirectionWrite().FactorySetZ(0.0f);
	return;
}