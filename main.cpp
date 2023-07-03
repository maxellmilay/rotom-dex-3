#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "./helper/utils.h"
#include "./components/PreviewCard.h"
#include "./components/Card.h"
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;

int main(){
    RenderWindow window(VideoMode(1920, 1080),"Rotom Dex");

    View view(FloatRect(0, 0, 1920, 1080));
    window.setView(view);

    const float scrollLimitTop = 0.0f;
    const float scrollLimitBottom = 20000.0f;

    Texture loadingDexTexture;
    loadingDexTexture.loadFromFile("./assets/images/pokedex-intro.png");

    Sprite loadingDex;
    loadingDex.setTexture(loadingDexTexture);

    FloatRect spriteBounds = loadingDex.getLocalBounds();
    loadingDex.setOrigin(spriteBounds.width / 2, spriteBounds.height / 2);
    loadingDex.setPosition(window.getSize().x / 2, window.getSize().y / 2);

    Font signikaNegative;
    signikaNegative.loadFromFile("./assets/fonts/SignikaNegative.ttf");
    string path = "./assets/pokemon/info";
    int pokemonCount = countPokemon(path);

    //create a pokemon list pointer array
    string** pokemonList = new string*[pokemonCount];
    for (int i = 0; i < pokemonCount; ++i) {
        pokemonList[i] = new string[POKEMON_INFO_COUNT];
    }

    //fill in pokemon list
    scanFilesInFolder(path, pokemonList);

    //max row and column of preview grid
    int numRows;
    int numCols;

    //dropdown
    std::string selectedOption = "All";
    bool isDropdownOpen = false;

    sf::RectangleShape dropdownButton(sf::Vector2f(200, 50));
    dropdownButton.setPosition(425, 340);
    dropdownButton.setFillColor(sf::Color::White);
    dropdownButton.setOutlineThickness(1);
    dropdownButton.setOutlineColor(sf::Color::Black);

    sf::Text dropdownText(selectedOption, signikaNegative, 30);
    dropdownText.setPosition(430, 345);
    dropdownText.setFillColor(sf::Color::Black);

    sf::RectangleShape dropdownList;
    dropdownList.setFillColor(sf::Color::White);
    dropdownList.setOutlineThickness(1);
    dropdownList.setOutlineColor(sf::Color::Black);

    //dropdown type buttons
    sf::Text option1Text("Fire", signikaNegative, 30);
    option1Text.setFillColor(sf::Color::Black);
    sf::Text option2Text("Normal", signikaNegative, 30);
    option2Text.setFillColor(sf::Color::Black);
    sf::Text option3Text("Grass", signikaNegative, 30);
    option3Text.setFillColor(sf::Color::Black);
    sf::Text option4Text("Water", signikaNegative, 30);
    option4Text.setFillColor(sf::Color::Black);
    sf::Text option5Text("Ground", signikaNegative, 30);
    option5Text.setFillColor(sf::Color::Black);
    sf::Text option6Text("Rock", signikaNegative, 30);
    option6Text.setFillColor(sf::Color::Black);
    sf::Text option7Text("Bug", signikaNegative, 30);
    option7Text.setFillColor(sf::Color::Black);
    sf::Text option8Text("Electric", signikaNegative, 30);
    option8Text.setFillColor(sf::Color::Black);
    sf::Text option9Text("Ice", signikaNegative, 30);
    option9Text.setFillColor(sf::Color::Black);
    sf::Text option10Text("Fighting", signikaNegative, 30);
    option10Text.setFillColor(sf::Color::Black);
    sf::Text option11Text("Poison", signikaNegative, 30);
    option11Text.setFillColor(sf::Color::Black);
    sf::Text option12Text("Flying", signikaNegative, 30);
    option12Text.setFillColor(sf::Color::Black);
    sf::Text option13Text("Psychic", signikaNegative, 30);
    option13Text.setFillColor(sf::Color::Black);
    sf::Text option14Text("Dark", signikaNegative, 30);
    option14Text.setFillColor(sf::Color::Black);
    sf::Text option15Text("Dragon", signikaNegative, 30);
    option15Text.setFillColor(sf::Color::Black);
    sf::Text option16Text("Ghost", signikaNegative, 30);
    option16Text.setFillColor(sf::Color::Black);
    sf::Text option17Text("Steel", signikaNegative, 30);
    option17Text.setFillColor(sf::Color::Black);
    sf::Text option18Text("Fairy", signikaNegative, 30);
    option18Text.setFillColor(sf::Color::Black);
    sf::Text option19Text("All", signikaNegative, 30);
    option19Text.setFillColor(sf::Color::Black);

    //SEARCH INPUT
    RectangleShape textContainer;
    textContainer.setSize({300,75});
    textContainer.setFillColor(Color::White);
    textContainer.setPosition(1250,200);

    Text inputText;
    inputText.setString("");
    inputText.setFillColor(Color::Black);
    inputText.setFont(signikaNegative);
    inputText.setCharacterSize(25);
    inputText.setOrigin(0,inputText.getGlobalBounds().height/2);
    inputText.setPosition(textContainer.getPosition().x + 50, textContainer.getPosition().y + textContainer.getGlobalBounds().height/2);

    //SEARCH INPUT

    //current pokemon displayed
    string currentPokemon;

    //scene indicator
    int activeScene = 1;

    //indicator if a type filter from dropdown is selected
    bool isFilterActive = false;

    int scrollCount = 0;

    bool isSearchActive = false;

    while(window.isOpen()){
        Event event;
        View currentView = window.getView();
        
        if(activeScene == 1 || activeScene == 3){
            currentView.setCenter(window.getSize().x / 2, window.getSize().y / 2);
        }

        currentView.setSize(window.getSize().x, window.getSize().y);
        window.setView(currentView);

        string** filteredPokemonList = new string*[pokemonCount];
        int filteredCount = 0;

        //filter pokemon per type
        if(isFilterActive){  
            for (int i = 0; i < pokemonCount; ++i) {
                if (createSubstring(pokemonList[i][2],',') == selectedOption) { 
                    filteredPokemonList[filteredCount] = new string[POKEMON_INFO_COUNT];
                    for (int j = 0; j < POKEMON_INFO_COUNT; ++j) {
                        filteredPokemonList[filteredCount][j] = pokemonList[i][j];
                    }
                    filteredCount++;
                }
            }

            numRows = (filteredCount + 3) / 4;
            numCols = 4;
        } else {
            numRows = (pokemonCount + 3) / 4;
            numCols = 4;
        }

        PreviewCard** PreviewCardList = new PreviewCard*[numRows];
        int listCounter = 0;

        for (int i = 0; i < numRows; i++) {
            PreviewCardList[i] = new PreviewCard[numCols];
            for (int j = 0; j < numCols; j++) {
                if (isFilterActive) {
                    if (listCounter < filteredCount) {
                        string name = filteredPokemonList[listCounter][1];
                        string number = filteredPokemonList[listCounter][0];
                        string type = filteredPokemonList[listCounter][2];

                        PreviewCard pcard(name, number, type, signikaNegative);
                        pcard.setPosition(i, j);

                        PreviewCardList[i][j] = pcard;
                        listCounter++;
                    }
                } else {
                    if (listCounter < pokemonCount) {
                        string name = pokemonList[listCounter][1];
                        string number = pokemonList[listCounter][0];
                        string type = createSubstring(pokemonList[listCounter][2],',');

                        PreviewCard pcard(name,number,type,signikaNegative);
                        pcard.setPosition(i,j);

                        PreviewCardList[i][j] = pcard;
                        listCounter++;
                    }
                }
            }
        }

        string Name;
        string number;
        string types[2];
        string description;
        string weaknesses[6];
        string evolution[3];

        for (int i = 0; i < pokemonCount; i++) {
            if (toLowercase(pokemonList[i][1]) == toLowercase(currentPokemon)) {
                Name = toLowercase(pokemonList[i][1]);
                number = toLowercase(pokemonList[i][0]);

                // Types
                std::vector<std::string> typeVec;
                std::istringstream issType(pokemonList[i][2]);
                std::string typeToken;

                while (std::getline(issType, typeToken, ',')) {
                    typeVec.push_back(toLowercase(typeToken));
                }

                // Populate types array
                for (int j = 0; j < 2 && j < typeVec.size(); j++) {
                    types[j] = typeVec[j];
                }

                // Weaknesses
                std::vector<std::string> weakVec;
                std::istringstream issWeak(pokemonList[i][3]);
                std::string weakToken;

                while (std::getline(issWeak, weakToken, ',')) {
                    weakVec.push_back(toLowercase(weakToken));
                }

                // Populate weaknesses array
                for (int j = 0; j < 6 && j < weakVec.size(); j++) {
                    weaknesses[j] = weakVec[j];
                }

                // Evolution
                std::vector<std::string> evoVec;
                std::istringstream issEvo(pokemonList[i][4]); // Assuming evolution data is at index 4
                std::string evoToken;

                while (std::getline(issEvo, evoToken, ',')) {
                    evoVec.push_back(toLowercase(evoToken));
                }

                // Populate evolution array
                for (int j = 0; j < 3 && j < evoVec.size(); j++) {
                    evolution[j] = evoVec[j];
                }

                description = pokemonList[i][5];
                break;
            }
        }

        Card currentCard(Name, number, types, weaknesses, description, evolution, signikaNegative);
    
        while(window.pollEvent(event)){
            if(event.type==Event::Closed){
                window.close();
                break;
            } else if (event.type == Event::MouseButtonPressed){
                if (event.mouseButton.button == Mouse::Left){
                    Vector2f mousePosition = window.mapPixelToCoords(sf::Vector2i(event.mouseButton.x, event.mouseButton.y));
                    
                    //scene 1 clicks
                    if(activeScene == 1){
                        if (loadingDex.getGlobalBounds().contains(mousePosition)) {
                            activeScene = 2;
                        }
                    }

                    //scene 2 clicks
                    else if(activeScene == 2){
                        //search bar clicks
                        if(textContainer.getGlobalBounds().contains(mousePosition)){
                            isSearchActive = true;
                            if(!isLastCharacterEqualTo(inputText.getString(),'_')){
                                inputText.setString(inputText.getString()+"_");
                            }
                        } else {
                            isSearchActive = false;
                            string currentString = inputText.getString();
                            if(isLastCharacterEqualTo(currentString,'_')){
                                inputText.setString(currentString.erase(currentString.size()-1));
                            }
                        }

                        //dropdown clicks
                        if (dropdownButton.getGlobalBounds().contains(mousePosition)) {
                        isDropdownOpen = !isDropdownOpen;
                        } else if (isDropdownOpen && dropdownList.getGlobalBounds().contains(mousePosition)) {
                            if (option1Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Fire";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option2Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Normal";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option3Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Grass";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option4Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Water";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option5Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Ground";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option6Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Rock";
                                isFilterActive = true;
                        
                                dropdownText.setString(selectedOption);
                            } else if (option7Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Bug";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option8Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Electric";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option9Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Ice";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option10Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Fighting";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option11Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Poison";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option12Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Flying";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option13Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Psychic";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option14Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Dark";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option15Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Dragon";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option16Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Ghost";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option17Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Steel";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option18Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "Fairy";
                                isFilterActive = true;
                            
                                dropdownText.setString(selectedOption);
                            } else if (option19Text.getGlobalBounds().contains(mousePosition)) {
                                selectedOption = "All";
                                dropdownText.setString(selectedOption);
                                isFilterActive = false;
                            }
                            isDropdownOpen = false;
                        } else {
                            isDropdownOpen = false;
                        }

                        //preview card clicks
                        for(int i = 0; i < numRows; i++){
                            for(int j = 0; j < numCols; j++){
                                if(PreviewCardList[i][j].getBounds().contains(mousePosition)) {
                                    currentPokemon = PreviewCardList[i][j].getName();
                                    activeScene = 3;
                                }
                            }
                        }
                    }

                    //scene 3 mouse clicks
                    else if(activeScene == 3){
                        if (currentCard.getBackButton().getGlobalBounds().contains(mousePosition)){
                            activeScene = 2;
                            currentView.setCenter(window.getSize().x / 2, (window.getSize().y / 2) + scrollCount);
                            window.setView(view);
                            scrollCount = 0;
                        }
                    }
                }
            } else if (event.type == Event::MouseWheelScrolled) {
                //scene 2 scroll events
                if(activeScene == 2){
                    if (event.mouseWheelScroll.delta > 0){
                        if (view.getCenter().y - view.getSize().y / 2 > scrollLimitTop)
                            view.move(0, -100);
                            scrollCount -= 100;
                    } else {
                        if (view.getCenter().y + view.getSize().y / 2 < scrollLimitBottom)
                            view.move(0, 100);
                            scrollCount += 100;
                    }
                    window.setView(view);
                }
            } else if (event.type == Event::Key)
        }

        if (isDropdownOpen) {
            dropdownList.setSize(sf::Vector2f(dropdownButton.getSize().x, 580));
            dropdownList.setPosition(dropdownButton.getPosition().x, dropdownButton.getPosition().y + dropdownButton.getSize().y);
            option1Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 5);
            option2Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 35);
            option3Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 65);
            option4Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 95);
            option5Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 125);
            option6Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 155);
            option7Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 185);
            option8Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 215);
            option9Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 245);
            option10Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 275);
            option11Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 305);
            option12Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 335);
            option13Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 365);
            option14Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 395);
            option15Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 425);
            option16Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 455);
            option17Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 485);
            option18Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 515);
            option19Text.setPosition(dropdownList.getPosition().x + 5, dropdownList.getPosition().y + 545);
        }

        window.clear();

        if(activeScene == 1){
            window.draw(loadingDex);
        } else if(activeScene == 2){
            window.draw(dropdownButton);
            window.draw(dropdownText);
            window.draw(textContainer);
            window.draw(inputText);
            
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    PreviewCardList[i][j].drawTo(window);
                }
            }

            if (isDropdownOpen) {
                window.draw(dropdownList);
                window.draw(option1Text);
                window.draw(option2Text);
                window.draw(option3Text);
                window.draw(option4Text);
                window.draw(option5Text);
                window.draw(option6Text);
                window.draw(option7Text);
                window.draw(option8Text);
                window.draw(option9Text);
                window.draw(option10Text);
                window.draw(option11Text);
                window.draw(option12Text);
                window.draw(option13Text);
                window.draw(option14Text);
                window.draw(option15Text);
                window.draw(option16Text);
                window.draw(option17Text);
                window.draw(option18Text);
                window.draw(option19Text);
            }
        } else if (activeScene == 3){
            for(int i = 0; i < pokemonCount; i++){
                if(pokemonList[i][1] == currentPokemon){
                    currentCard.drawTo(window);
                }
            }
        }
        window.display();

    }
}
