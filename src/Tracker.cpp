/** License GPLv3
  * @author Mer Alba / merAlba
  */
#include <tracker_movement/Tracker.h>

Tracker::Tracker(){
    this->max_users= 10;
    this->skeletonStates = vector<nite::SkeletonState>(max_users,nite::SKELETON_NONE);
    this->visibleUsers = vector<bool>(10,false);
    this->printPoints = false;
    this->isFront = false;
    this->pose = none;
    this->manos = false;
    this->userTracker = new nite::UserTracker();
}

Tracker::Tracker(bool points, int num_users){
    this->max_users = num_users;
    this->skeletonStates = vector<nite::SkeletonState>(max_users, nite::SKELETON_NONE);
    this->visibleUsers = vector<bool>(num_users,false);
    this->printPoints = points;
    this->isFront = false;
    this->pose = none;
    this->manos = false;
    this->userTracker = new nite::UserTracker();
}

Tracker::~Tracker(){
    nite::NiTE::shutdown();
}

nite::UserTrackerFrameRef Tracker::getUserTrackerFrame(){
    return userTrackerFrame;
}

nite::UserTracker* Tracker::getUserTracker(){
    return userTracker;
}


void Tracker::updateUserState(const nite::UserData& user, unsigned long long ts)
{
	if (user.isNew())
		ROS_INFO_STREAM("New");
	else if (user.isVisible() && !visibleUsers[user.getId()])
		ROS_INFO_STREAM("Visible");
	else if (!user.isVisible() && visibleUsers[user.getId()])
		ROS_INFO_STREAM("Out of Scene");
	else if (user.isLost())
		ROS_INFO_STREAM("Lost");

    visibleUsers[user.getId()] = user.isVisible();


	if(skeletonStates[user.getId()] != user.getSkeleton().getState())
	{
		switch(skeletonStates[user.getId()] = user.getSkeleton().getState())
		{
		case nite::SKELETON_NONE:
			ROS_INFO_STREAM("Stopped tracking.");
			break;
		case nite::SKELETON_CALIBRATING:
			ROS_INFO_STREAM("Calibrating...");
			break;
		case nite::SKELETON_TRACKED:
			ROS_INFO_STREAM("Tracking!");
			break;
		case nite::SKELETON_CALIBRATION_ERROR_NOT_IN_POSE:
		case nite::SKELETON_CALIBRATION_ERROR_HANDS:
		case nite::SKELETON_CALIBRATION_ERROR_LEGS:
		case nite::SKELETON_CALIBRATION_ERROR_HEAD:
		case nite::SKELETON_CALIBRATION_ERROR_TORSO:
			ROS_INFO_STREAM("Calibration Failed... :-|");
			break;
		}
	}
}

bool Tracker::checkConfidence(const nite::UserData& user){
    bool confidence = false;
    const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
    const nite::SkeletonJoint& neck = user.getSkeleton().getJoint(nite::JOINT_NECK);
    const nite::SkeletonJoint& right_shoulder = user.getSkeleton().getJoint(nite::JOINT_RIGHT_SHOULDER);
    const nite::SkeletonJoint& right_elbow = user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW);
    const nite::SkeletonJoint& right_hand = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND);
    const nite::SkeletonJoint& left_shoulder = user.getSkeleton().getJoint(nite::JOINT_LEFT_SHOULDER);
    const nite::SkeletonJoint& left_elbow = user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW);
    const nite::SkeletonJoint& left_hand = user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND);
    const nite::SkeletonJoint& torso = user.getSkeleton().getJoint(nite::JOINT_TORSO);
    const nite::SkeletonJoint& left_hip = user.getSkeleton().getJoint(nite::JOINT_LEFT_HIP);
    const nite::SkeletonJoint& right_hip = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HIP);
    const nite::SkeletonJoint& left_knee = user.getSkeleton().getJoint(nite::JOINT_LEFT_KNEE);
    const nite::SkeletonJoint& right_knee = user.getSkeleton().getJoint(nite::JOINT_RIGHT_KNEE);
    if (head.getPositionConfidence() >.5 && neck.getPositionConfidence() >.5 &&
            right_shoulder.getPositionConfidence() > .5 && right_elbow.getPositionConfidence() >.5 && right_hand.getPositionConfidence() >.5 &&
            left_shoulder.getPositionConfidence() >.5 && left_elbow.getPositionConfidence() >.5 && left_hand.getPositionConfidence() >.5 &&
            torso.getPositionConfidence() >.5 && left_hip.getPositionConfidence() >.5 && right_hip.getPositionConfidence() >.5 &&
            left_knee.getPositionConfidence() >.5 && right_knee.getPositionConfidence() >.5 ){
                confidence = true;
            }
    return confidence;
}

void Tracker::showPoints(const nite::UserData& user){
    const nite::SkeletonJoint& head = user.getSkeleton().getJoint(nite::JOINT_HEAD);
    const nite::SkeletonJoint& neck = user.getSkeleton().getJoint(nite::JOINT_NECK);
    const nite::SkeletonJoint& right_shoulder = user.getSkeleton().getJoint(nite::JOINT_RIGHT_SHOULDER);
    const nite::SkeletonJoint& right_elbow = user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW);
    const nite::SkeletonJoint& right_hand = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND);
    const nite::SkeletonJoint& left_shoulder = user.getSkeleton().getJoint(nite::JOINT_LEFT_SHOULDER);
    const nite::SkeletonJoint& left_elbow = user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW);
    const nite::SkeletonJoint& left_hand = user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND);
    const nite::SkeletonJoint& torso = user.getSkeleton().getJoint(nite::JOINT_TORSO);
    const nite::SkeletonJoint& left_hip = user.getSkeleton().getJoint(nite::JOINT_LEFT_HIP);
    const nite::SkeletonJoint& right_hip = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HIP);
    const nite::SkeletonJoint& left_knee = user.getSkeleton().getJoint(nite::JOINT_LEFT_KNEE);
    const nite::SkeletonJoint& right_knee = user.getSkeleton().getJoint(nite::JOINT_RIGHT_KNEE);

    ROS_INFO("Cabeza[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), head.getPosition().x, head.getPosition().y, head.getPosition().z);
    ROS_INFO("Cuello[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), neck.getPosition().x, neck.getPosition().y, neck.getPosition().z);
    ROS_INFO("Hombro derecho[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), right_shoulder.getPosition().x, right_shoulder.getPosition().y, right_shoulder.getPosition().z);
    ROS_INFO("Codo derecho[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), right_elbow.getPosition().x, right_elbow.getPosition().y, right_elbow.getPosition().z);
    ROS_INFO("Mano derecha[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), right_hand.getPosition().x, right_hand.getPosition().y, right_hand.getPosition().z);
    ROS_INFO("Hombro izquierdo[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), left_shoulder.getPosition().x, left_shoulder.getPosition().y, left_shoulder.getPosition().z);
    ROS_INFO("Codo izquierdo[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), left_elbow.getPosition().x, left_elbow.getPosition().y, left_elbow.getPosition().z);
    ROS_INFO("Mano izquierda[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), left_hand.getPosition().x, left_hand.getPosition().y, left_hand.getPosition().z);
    ROS_INFO("Torso[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), torso.getPosition().x, torso.getPosition().y, torso.getPosition().z);
    ROS_INFO("Cadera derecha[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), right_hip.getPosition().x, right_hip.getPosition().y, right_hip.getPosition().z);
    ROS_INFO("Cadera izquierda[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), left_hip.getPosition().x, left_hip.getPosition().y, left_hip.getPosition().z);
    ROS_INFO("Rodilla derecha[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), right_knee.getPosition().x, right_knee.getPosition().y, right_knee.getPosition().z);
    ROS_INFO("Rodilla izquierda[User:%d (%5.2f, %5.2f, %5.2f)]", user.getId(), left_knee.getPosition().x, left_knee.getPosition().y, left_knee.getPosition().z);

}

bool Tracker::userFront(const nite::UserData &user){
    bool front = false;
    const nite::SkeletonJoint& right_shoulder = user.getSkeleton().getJoint(nite::JOINT_RIGHT_SHOULDER);
    const nite::SkeletonJoint& left_shoulder = user.getSkeleton().getJoint(nite::JOINT_LEFT_SHOULDER);
    const nite::SkeletonJoint& right_hip = user.getSkeleton().getJoint(nite::JOINT_RIGHT_HIP);
    const nite::SkeletonJoint& left_hip = user.getSkeleton().getJoint(nite::JOINT_LEFT_HIP);
    const nite::SkeletonJoint& neck = user.getSkeleton().getJoint(nite::JOINT_NECK);
    const nite::SkeletonJoint& torso = user.getSkeleton().getJoint(nite::JOINT_TORSO);

    if ( abs(right_shoulder.getPosition().y-left_shoulder.getPosition().y) <= 40 &&
        abs(right_hip.getPosition().y-left_hip.getPosition().y) <= 40 &&
        abs(neck.getPosition().x-torso.getPosition().x) <= 40 )

    {
        front = true;
    }

    return front;

}

 nite::Status Tracker::run(std_msgs::String& cmdGripper, nite::UserData& user){
     niteStatus = userTracker->readFrame(&userTrackerFrame);
     if (niteStatus == nite::STATUS_OK)
     {
         const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers(); //Vector de datos de todos los usuarios encontrados por la kinect

         if ( users.getSize() == 1 ){
             user = users[0];

             //updateUserState(user,userTrackerFrame.getTimestamp());
             if (user.isNew())
             {
                 userTracker->startSkeletonTracking(user.getId());
                 isFront = false;
                 ROS_INFO_STREAM("Found a new user.");
             }
             else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED && checkConfidence(user))
             {

                 if(printPoints){
                     showPoints(user);
                 }
                 if ( !isFront && userFront(user) ) {
                     referencePoints.setReferencePoints(user);
                     fuzzySystem.setFuzzySystem(referencePoints);

                     //Llamar a los sistemas difusos
                     isFront = true;
                     ROS_INFO_STREAM("Reference points have been established");
                 }
                 if( isFront ) {
                     std::stringstream ss;

                     if( pose != center && pose != high && pose != down && fuzzySystem.processSameHeightHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y) > 0.5 ){
                         if( pose == centerHands ){
                             // ROS_INFO_STREAM(" Valor: " << user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x << " "
                             //                                 << user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x << " "
                             //                                     << user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y << " | "
                             //                                     << fuzzySystem.processSeparateHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                             //                                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                             //                                                                         user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ));
                              if( fuzzySystem.processSeparateHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){
                                 pose = separate;
                                 ss << "abrir";

                                 ROS_DEBUG_STREAM("abrir");
                                 cmdGripper.data = ss.str();
                             }
                             else{
                                 if( fuzzySystem.processCloseHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){
                                     pose = closed;
                                     ss << "cerrar";

                                     ROS_DEBUG_STREAM("cerrar");
                                     cmdGripper.data = ss.str();
                                 }
                                 /*else{ //new
                                     if( fuzzySystem.processHighRightHandEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
                                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y) > 0.5 ){
                                         if( pose == centerHands ) //centerBothHands {
                                             pose = highHands;
                                             ss << "avanzar";
                                             cmdGripper.data = ss.str();
                                         }
                                     }
                                     else{
                                         if ( pose == highHands ){
                                             pose = none;
                                             ss << "parar";
                                             cmdGripper.data = ss.str();
                                         }
                                         if ( fuzzySystem.processDownRightHandEngine( user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
                                                                                     user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                                                                                     user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y) > 0.5 ){
                                             if (pose == centerHands){
                                                 pose = downHands;
                                                 ss << "retroceder";
                                                 cmdGripper.data = ss.str();
                                             }
                                         }
                                         else {
                                             if ( pose == downHands ){
                                                 pose = none;
                                                 ss << "parar";
                                                 cmdGripper.data = ss.str();
                                             }
                                         }
                                     }
                                 }
                                 */
                             }
                         }
                         else{

                             if( fuzzySystem.processCenterHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){

                                 if (pose != centerHands /*centerBothHands*/){
                                     pose = centerHands; /*centerBothHands*/

                                     ROS_DEBUG_STREAM("centradas");
                                 }
                             }
                         }
                     }
                     else{

                         //Comprobar movimiento arriba
                         if( pose == center || pose == high || pose == down ){
                             if ( fuzzySystem.processHighLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                                                         user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                         user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y ) > 0.5 ){
                                 if( pose == center){ //1º vez
                                     pose = high;
                                     ss << "subir";
                                     // printf("leftHand_y: %f   leftHand_x: %f   leftElbow_y: %f\n", user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                     //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                     //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y);
                                     ROS_DEBUG_STREAM("subir");
                                     cmdGripper.data = ss.str();
                                 }
                                 // printf("leftHand_y: %f   leftHand_x: %f   leftElbow_y: %f\n", user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                 //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                 //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y);

                             }
                             else {
                                 if ( pose == high ){
                                     pose = none;
                                     ss << "parar pinza";
                                     // printf("leftHand_y: %f   leftHand_x: %f   leftElbow_y: %f\n", user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                     //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                     //                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y);

                                     ROS_DEBUG_STREAM("parar pinza");
                                     cmdGripper.data = ss.str();
                                 }
                                 if ( fuzzySystem.processDownLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y ) > 0.5 ){


                                     if (pose == center){
                                         pose = down;
                                         ss << "bajar";

                                         ROS_DEBUG_STREAM("bajar");
                                         cmdGripper.data = ss.str();
                                     }
                                 }
                                 else {
                                     if ( pose == down ){
                                         pose = none;
                                         ss << "parar pinza";

                                         ROS_DEBUG_STREAM("parar pinza");
                                         cmdGripper.data = ss.str();
                                     }
                                 }
                             }
                         }
                         else{

                              if( fuzzySystem.processCenterLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
                                                                                 user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
                                                                                 user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y) > 0.5 ){
                                 if( pose != center ){
                                    ROS_DEBUG_STREAM("centrada");
                                     pose = center;
                                 }
                             }
                             else{
                                 if( pose != none ){
                                     ROS_DEBUG_STREAM("ninguna1");
                                     pose = none;
                                 }
                             }
                         }
                     }
                 }
             }
 //			publishTransforms(frame_id);
         }
         else {

             if(users.getSize() > 1){
                 ROS_WARN_STREAM("E.1-There're " << users.getSize() << " users in scene. This program only support one user");
             }
         }
     }
     else
     {
         ROS_WARN_STREAM("Get next frame failed.");
     }

     return niteStatus;
 }
//
// nite::Status Tracker::run(string& action){ //new
//     niteStatus = userTracker->readFrame(&userTrackerFrame);
//     if (niteStatus == nite::STATUS_OK)
//     {
//         const nite::Array<nite::UserData>& users = userTrackerFrame.getUsers(); //Vector de datos de todos los usuarios encontrados por la kinect
//
//         if ( users.getSize() == 1 ){
//             const nite::UserData& user = users[0];
//
//             //updateUserState(user,userTrackerFrame.getTimestamp());
//             if (user.isNew())
//             {
//                 userTracker->startSkeletonTracking(user.getId());
//                 isFront = false;
//                 ROS_INFO_STREAM("Found a new user.");
//             }
//             else if (user.getSkeleton().getState() == nite::SKELETON_TRACKED && checkConfidence(user))
//             {
//
//                 if(printPoints){
//                     showPoints(user);
//                 }
//                 if ( !isFront && userFront(user) ) {
//                     referencePoints.setReferencePoints(user);
//                     fuzzySystem.setFuzzySystem(referencePoints);
//
//                     //Llamar a los sistemas difusos
//                     isFront = true;
//                     ROS_INFO_STREAM("Reference points have been established");
//                 }
//                 if( isFront ) {
//                     if( pose != center && pose != high && pose != down && fuzzySystem.processSameHeightHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
//                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y) > 0.5 ){
//                         if( pose == centerHands ){
//                             if( fuzzySystem.processSeparateHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
//                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){
//                                 pose = separate;
//                                 action = "abrir"; //new
//                                 ROS_WARN_STREAM("abiertas");
//
//                             }
//                             else{
//
//                                 if( fuzzySystem.processCloseHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
//                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){
//                                     pose = closed;
//                                     action = "cerrar";
//                                     ROS_WARN_STREAM("cerradas");
//                                 }
//                                 else{ //new
//                                     if( fuzzySystem.processHighRightHandEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
//                                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
//                                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y) > 0.5 ){
//                                         if( pose == centerHands ){ //centerBothHands
//                                             pose = highHands;
//                                             action = "avanzar";
//                                             ROS_WARN_STREAM("ambas arriba");
//                                         }
//                                     }
//                                     else{
//                                         if ( pose == highHands ){
//                                             pose = none;
//                                             action = "parar";
//                                             ROS_WARN_STREAM("parar movimiento");
//                                         }
//                                         if ( fuzzySystem.processDownRightHandEngine( user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y,
//                                                                                     user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
//                                                                                     user.getSkeleton().getJoint(nite::JOINT_RIGHT_ELBOW).getPosition().y) > 0.5 ){
//                                             if (pose == centerHands){
//                                                 pose = downHands;
//                                                 action = "retroceder";
//                                                 ROS_WARN_STREAM("ambas abajo");
//                                             }
//                                         }
//                                         else {
//                                             if ( pose == downHands ){
//                                                 pose = none;
//                                                 action = "parar";
//                                                 ROS_WARN_STREAM("parar movimiento");
//                                             }
//                                         }
//                                     }
//                                 }
//                             }
//                         }
//                         else{
//                             if( fuzzySystem.processCenterHandsEngine(user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().x,
//                                                                     user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                 user.getSkeleton().getJoint(nite::JOINT_RIGHT_HAND).getPosition().y ) > 0.5){
//
//                                 if (pose != centerHands /*centerBothHands*/){
//                                     pose = centerHands; /*centerBothHands*/
//                                     ROS_WARN_STREAM("ambas centradas");
//                                 }
//                             }
//                         }
//                     }
//                     else{
//                         //Comprobar movimiento arriba
//                         if( pose == center || pose == high || pose == down ){
//                             if ( fuzzySystem.processHighLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
//                                                                         user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                         user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y ) > 0.5 ){
//                                 if( pose == center){ //1º vez
//                                     pose = high;
//                                     action = "subir";
//                                     ROS_WARN_STREAM("arriba");
//                                 }
//                             }
//                             else {
//                                 if ( pose == high ){
//                                     pose = none;
//                                     action = "parar pinza";
//                                     ROS_WARN_STREAM("parar pinza");
//                                 }
//                                 if ( fuzzySystem.processDownLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
//                                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                             user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y ) > 0.5 ){
//
//
//                                     if (pose == center){
//                                         pose = down;
//                                         action = "bajar";
//                                         ROS_WARN_STREAM("bajada");
//                                     }
//                                 }
//                                 else {
//                                     if ( pose == down ){
//                                         pose = none;
//                                         action = "parar pinza";
//                                         ROS_WARN_STREAM("parar pinza");
//                                     }
//                                 }
//                             }
//                         }
//                         else{
//                              if( fuzzySystem.processCenterLeftHandEngine( user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().y,
//                                                                                 user.getSkeleton().getJoint(nite::JOINT_LEFT_HAND).getPosition().x,
//                                                                                 user.getSkeleton().getJoint(nite::JOINT_LEFT_ELBOW).getPosition().y) > 0.5 ){
//                                 if( pose != center ){
//                                     pose = center;
//
//                                     ROS_WARN_STREAM("centrada");
//                                 }
//                             }
//                             else{
//                                 if( pose != none ){
//                                     pose = none;
//
//                                     ROS_WARN_STREAM("none");
//                                 }
//                             }
//                         }
//                     }
//                 }
//             }
//	//		publishTransforms(frame_id);
//         }
//         else {
//             if(users.getSize() > 1){    //TODO in SCENE
//                 ROS_WARN_STREAM("E.1-There's more than one user in scene");
//                 cout << users.getSize()<<endl;
//             }
//         }
//     }
//     else
//     {
//         ROS_WARN_STREAM("Get next frame failed.");
//     }
//     return niteStatus;
// }


int Tracker::getMaxUsers(){
    return this->max_users;
}
