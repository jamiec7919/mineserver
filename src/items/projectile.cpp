#include "projectile.h"
#include "../packets.h"
#include <cmath>


uint32_t generateEID();

bool ItemProjectile::affectedItem(int item)
{
  switch(item){
  case ITEM_SNOWBALL:
  case ITEM_EGG:
  case ITEM_BOW:
    return true;
  }
  return false;
}

void ItemProjectile::onRightClick(User* user, Item* item)
{
  int8_t projID = 0;
  switch(item->getType())
  {
    case ITEM_SNOWBALL:
      projID = 61;
    break;
    case ITEM_EGG:
      projID = 62;
    break;
    case ITEM_BOW:
      projID = 60; // TODO check arrows and remove one!
    break;
  }
  if(projID != 0)
  {
    int32_t EID = generateEID();
    Packet pkt;
    //Entity packet (initialization)
    pkt << 0x1E << EID;

    //Spawn snowball on player location
    // 0x17 == add object, 61 == snowball
    pkt << 0x17 << EID << (int8_t)projID << (int32_t)(user->pos.x*32) << (int32_t)((user->pos.y+1.5)*32) << (int32_t)(user->pos.z*32);
  
    float tempMult = 1.f-((user->pos.pitch/90.f)<0?-(user->pos.pitch/90.f):(user->pos.pitch/90.f));
    //Entity velocity 
    pkt << 0x1C << EID << (int16_t)(sinf(-(user->pos.yaw / 360.f) * 2.f*M_PI)*tempMult*32768.f) << (int16_t)(-32768.f*(user->pos.pitch/90.f)) << (int16_t)(cosf(-(user->pos.yaw/ 360.f) * 2.f*M_PI)*tempMult*32768.f);
  
    user->sendAll((uint8_t *)pkt.getWrite(), pkt.getWriteLen());

    item->decCount();
  }
 
}

