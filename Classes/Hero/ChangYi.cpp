#include "ChangYi.h"
#include "Consts.h"
#include "Entity/AI.h"

Hero* ChangYi::createAI()
{
    auto player = AI::create("Hero/FightHero/ChangYi.png");

    player->health_ = ChangYiHealthInit;//当前生命

    player->healthInit_ = ChangYiHealthInit;//最大生命

    player->attack_ = ChangYiAttack;//攻击值

    player->defend_ = ChangYiDefend;//防御值

    return player;
}

ChangYi* ChangYi::createPlayer()
{
    auto player = new(std::nothrow) ChangYi;

    if (!player)
    {
        return nullptr;
    }
    player->dieTag_ = PlayerDieTag;

    player->health_ = ChangYiHealthInit;//当前生命

    player->healthInit_ = ChangYiHealthInit;//最大生命

    player->attack_ = ChangYiAttack;//攻击值

    player->defend_ = ChangYiDefend;//防御值

    auto chosenHero = Sprite::create("Hero/FightHero/ChangYi.png");

    if (chosenHero)
    {
        player->setPosition(player->x, player->y);// player初始坐标(x,y)根据窗口大小 在protected里面改

        player->bindPhysicsBodyAndTag(chosenHero, EnemyBulletAndPlayerBitmask, PlayerTag);

        player->addChild(chosenHero);

        player->autorelease();

        return player;
    }

    return nullptr;
}

void ChangYi::ultimateSkill(Vec2 touchWorldPosition)
{
    auto useSkill = Sprite::create("Hero/Bullet/ChangYi-bullet.png");

    useSkill->setPosition(touchWorldPosition);

    Hero::bindPhysicsBodyAndTag(useSkill, EnemyBulletAndPlayerBitmask, ChangYiUtimateSkillTag);

    this->getParent()->addChild(useSkill);

    this->confirm_ = false;// 大招释放完毕confirm_复原
}

void ChangYi::attackHero(Vec2 playerPosition,Vec2 touchWorldPosition)
{
    /* 创造currentBullet并设置初始位置 */
    auto currentBullet = Sprite::create("Hero/Bullet/ChangYi-bullet-usual.png");
    currentBullet->setPosition(playerPosition);// 初始位置是从currentPlayer出发

    /* 给currentBullet绑定物理躯干 */
    Hero::bindPhysicsBodyAndTag(currentBullet, PlayerBulletAndEnemyBitmask, PlayerBulletTag);

    /* 加入渲染树 */
    this->getParent()->addChild(currentBullet);

    Vec2 offset = touchWorldPosition - playerPosition;
    offset.normalize();// currentPlayer位置指向鼠标touch位置的单位向量

    /* 定义一些动作 */
    auto actionMove = MoveBy::create(1.5f, offset * ShootSpeed);// 1.5秒到达目的地
    auto actionRemove = RemoveSelf::create();// 删掉自身

    /* 让currentBullet完成上面的一系列动作 */
    currentBullet->runAction(Sequence::create(actionMove, actionRemove, nullptr));
}