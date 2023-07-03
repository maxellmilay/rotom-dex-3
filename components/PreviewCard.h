#include <SFML/Graphics.hpp>
#include <string.h>
#include "../helper/utils.h"

#define BOX_HEIGHT 210
#define BOX_WIDTH 210

using namespace std;
using namespace sf;

class PreviewCard{
    public:
        string type;
        PreviewCard(){
            //default
        };
        PreviewCard(string name, string number, string type, Font &font){
            this->type = type;
            this->name.setString(name);
            this->number.setString(number);

            this->name.setFont(font);
            this->number.setFont(font);

            this->container.setOutlineThickness(2.f);
            this->container.setOutlineColor(getColor(toLowercase(createSubstring(type,','))));
            this->container.setSize({BOX_WIDTH,BOX_HEIGHT});
            this->container.setFillColor(Color::Transparent);

            this->name.setFillColor(Color::White);
            this->name.setCharacterSize(20);
            FloatRect nameRect = this->name.getLocalBounds();
            this->name.setOrigin(nameRect.left + nameRect.width/2.0f,nameRect.top  + nameRect.height/2.0f);
       
            this->number.setFillColor(Color::White);
            this->number.setCharacterSize(20);
            FloatRect numberRect = this->number.getLocalBounds();
            this->number.setOrigin(numberRect.left + numberRect.width/2.0f,numberRect.top  + numberRect.height/2.0f);

            imgTexture.loadFromFile("./assets/pokemon/sprites/" + name + ".png");

        }

        void setPosition(float row, float col){
            float posX = 200*(col+1) + (col)*static_cast<float>(this->container.getSize().x);
            float posY = 500 + 100*(row+1) + (row)*static_cast<float>(this->container.getSize().y);
            
            this->container.setPosition({posX,posY});
            this->name.setPosition({posX + BOX_WIDTH/2,posY + 65});
            this->number.setPosition({posX + BOX_WIDTH/2,posY + 115});
            this->imgSprite.setPosition({posX + BOX_WIDTH/2,posY + 160});
        }

        void setFont(Font &font){
            number.setFont(font);
            name.setFont(font);
        }

        void drawTo(RenderWindow &window){
            window.draw(container);
            window.draw(name);
            window.draw(number);

            imgSprite.setTexture(this->imgTexture);
            this->imgSprite.setOrigin(this->imgSprite.getGlobalBounds().width / 2, this->imgSprite.getGlobalBounds().height / 2);
            
            window.draw(imgSprite);
        }

        string getName(){
            return name.getString();
        }

        FloatRect getBounds(){
            return this->container.getGlobalBounds();
        }

    private:
        Text name;
        Text number;
        RectangleShape container;
        Texture imgTexture;
        Sprite imgSprite;
};
