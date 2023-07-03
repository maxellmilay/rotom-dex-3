#include <SFML/Graphics.hpp>
#include <string.h>
#include <vector>
#include "../helper/utils.h"

#define LEFT_MARGIN 100

using namespace std;
using namespace sf;

class Card {
    public:
        Card(){
            //default
        }
        Card(string name, string number, string types[], string weaknesses[], string description, string evolution[], Font &font){
            //overloaded constructor setup
            this->name = name;
            this->number = number;
            this->description = description;
            
            for(int i = 0; i < 2; i++){
                this->types[i] = types[i];
            }

            for(int i = 0; i < 6; i++){
                this->weaknesses[i] = weaknesses[i];
            }
            for(  int i = 0; i < 3; i++){
                this->evolution[i] = evolution[i];
            }
            
            //file loading
            pokedex.loadFromFile("./assets/images/pokedex.png");
            pokedexSprite.setTexture(pokedex);

            backButton.loadFromFile("./assets/images/backButton.png");
            backButtonSprite.setTexture(backButton);

            deleteButton.loadFromFile("./assets/images/deleteButton.png");
            deleteButtonSprite.setTexture(deleteButton);

            editButton.loadFromFile("./assets/images/editButton.png");
            editButtonSprite.setTexture(editButton);

            if(this->name != ""){
                image.loadFromFile("./assets/pokemon/sprites/" + this->name + ".png");
                imageSprite.setTexture(image);
            }

            if(this->types[0] != ""){
                bg.loadFromFile("./assets/pokemon/bg/" + types[0] + ".png");
                bgSprite.setTexture(bg);
            }

            //background images properties
            backButtonSprite.setPosition({LEFT_MARGIN,100});
            backButtonSprite.setScale(0.5,0.5);

            pokedexSprite.setPosition({800 + 450,150});
            pokedexSprite.setScale(0.8,0.8);

            deleteButtonSprite.setPosition({850 + 450,840});
            deleteButtonSprite.setScale(0.2,0.2);

            editButtonSprite.setPosition({1150 + 450,850});
            editButtonSprite.setScale(0.15,0.15);
            editButtonSprite.setColor(Color::Black);

            bgSprite.setPosition({600,275});
            bgSprite.setScale(2.5,2.5);

            float centerX = bgSprite.getPosition().x + bgSprite.getGlobalBounds().width / 2;
            float centerY = bgSprite.getPosition().y + bgSprite.getGlobalBounds().height / 2;

            imageSprite.setOrigin(imageSprite.getGlobalBounds().width / 2, imageSprite.getGlobalBounds().height / 2);
            imageSprite.setPosition(centerX, centerY);
            imageSprite.setScale(4,4);   

            //background text setup
            nameText.setString(capitalizeFirstLetter(this->name));
            nameText.setFillColor(Color::White);
            nameText.setCharacterSize(50);
            nameText.setFont(font);
            nameText.setOrigin(nameText.getGlobalBounds().width / 2, 0);
            nameText.setPosition(centerX,150);

            numberText.setString("#" + this->number);
            numberText.setFillColor(Color::White);
            numberText.setCharacterSize(25);
            numberText.setFont(font);
            numberText.setOrigin(numberText.getGlobalBounds().width / 2, 0);
            numberText.setPosition(centerX,225);

            typeText.setString("Type");
            typeText.setFillColor(Color::White);
            typeText.setCharacterSize(35);
            typeText.setFont(font);
            typeText.setPosition(LEFT_MARGIN + 175,225);

            weaknessText.setString("Weakness");
            weaknessText.setFillColor(Color::White);
            weaknessText.setCharacterSize(35);
            weaknessText.setFont(font);
            weaknessText.setOrigin(weaknessText.getGlobalBounds().width / 2,0);
            weaknessText.setPosition(this->typeText.getPosition().x + this->typeText.getGlobalBounds().width / 2,500);

            //loop save weakness texture
            if(weaknesses[0] != ""){
                int weaknessCount = sizeof(this->weaknesses) / sizeof(this->weaknesses[0]);
                for(int i = 0; i < weaknessCount; i++){
                    Texture weaknessPic;
                    Sprite weaknessSprite;

                    weaknessPic.loadFromFile("./assets/pokemon/types/" + this->weaknesses[i] + ".png");
                    weaknessTextureList.push_back(weaknessPic);  
                }
            }

            //loop save type texture
            if(types[0] != ""){
                int typeCount = sizeof(this->types) / sizeof(this->types[0]);
                for(int i = 0; i < typeCount; i++){
                    Texture typePic;
                    Sprite typeSprite;

                    typePic.loadFromFile("./assets/pokemon/types/" + this->types[i] + ".png");
                    typeTextureList.push_back(typePic);                   
                }
            }

            //loop save evolution
            if(evolution[0] != ""){
                int evoCount = sizeof(this->evolution) / sizeof(this->evolution[0]);
                for(int i = 0; i < evoCount; i++){
                    Texture evoPic;
                    Sprite evoSprite;

                    evoPic.loadFromFile("./assets/pokemon/sprites/" + this->evolution[i] + ".png");
                    evoTextureList.push_back(evoPic);                   
                }
            }

        }

        //render function
        void drawTo(RenderWindow &window){
            window.draw(this->pokedexSprite);
            window.draw(this->backButtonSprite);
            window.draw(this->deleteButtonSprite);
            window.draw(this->editButtonSprite);
            window.draw(this->bgSprite);
            window.draw(this->imageSprite);
            window.draw(this->nameText);
            window.draw(this->numberText);
            window.draw(this->typeText);
            window.draw(this->weaknessText);

            int count = 1;
            for (const auto& typeTexture : typeTextureList) {
                Sprite typeSprite;
                typeSprite.setTexture(typeTexture);
                typeSprite.setOrigin(typeSprite.getGlobalBounds().width / 2,0);
                typeSprite.setPosition(this->typeText.getPosition().x + this->typeText.getGlobalBounds().width / 2,225 + count*50);
                typeSprite.setScale(0.5,0.5);
                window.draw(typeSprite);
                ++count;
            }

            count = 1;
            for (const auto& weaknessTexture : weaknessTextureList) {
                Sprite weaknessSprite;
                weaknessSprite.setTexture(weaknessTexture);
                weaknessSprite.setOrigin(weaknessSprite.getGlobalBounds().width / 2,0);
                weaknessSprite.setPosition(this->typeText.getPosition().x + this->typeText.getGlobalBounds().width / 2,500 + count*50);
                weaknessSprite.setScale(0.5,0.5);
                window.draw(weaknessSprite);
                ++count;
            }

            count = 1;
            for (const auto& evoTexture : evoTextureList) {
                Sprite evoSprite;
                evoSprite.setTexture(evoTexture);
                evoSprite.setOrigin(evoSprite.getGlobalBounds().width / 2,0);
                evoSprite.setPosition(LEFT_MARGIN + 450 + count*150, 825);
                evoSprite.setScale(3,3);
                window.draw(evoSprite);
                ++count;
            }
        }

        Sprite getBackButton(){
            return backButtonSprite;
        }

    private:
        string name;
        string number;
        string types[2];
        string weaknesses[6];
        string description;
        string evolution[3];

        vector<Texture> typeTextureList;
        vector<Texture> weaknessTextureList;
        vector<Texture> evoTextureList;

        //default card background images
        Texture pokedex;
        Sprite pokedexSprite;
        
        Texture backButton;
        Sprite backButtonSprite;

        Texture deleteButton;
        Sprite deleteButtonSprite;

        Texture editButton;
        Sprite editButtonSprite;

        //pokemon sprite image
        Texture image;
        Sprite imageSprite;

        Texture bg;
        Sprite bgSprite;

        //text entities
        Text nameText;
        Text numberText;
        Text typeText;
        Text weaknessText;

};
        
