// Created by Junior on 6/11/2016.
#ifndef ITEMSCREEN_H
#define ITEMSCREEN_H


class itemScreen
{
    public:
        enum ItemType { ItemHero, ItemEnemy, ItemPowerUp };

        itemScreen(ItemType);
        virtual ~itemScreen();
        virtual void init();

        virtual float *getPosition();
        float pos[3];
        float vel[3];

        ItemType itemType() {
            return screenItemType;
        }

        itemScreen *next;
        itemScreen *prev;

    protected:
        int age;

    private:
        ItemType screenItemType;
        static int itemCount;
};

#endif //ITEMSCREEN_H
