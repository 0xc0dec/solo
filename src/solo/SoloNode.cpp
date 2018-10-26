/*
    Copyright (c) Aleksey Fedotov
    MIT license
*/

#include "SoloNode.h"
#include "SoloScene.h"

using namespace solo;

Node::Node(Scene *scene, u32 nodeId):
    scene_(scene), id_(nodeId)
{
}
