#include <iostream>
#include <sstream>
#include <SFML/Graphics.hpp>
#include "./helper/utils.h"
#include "./components/PreviewCard.h"
#include "./components/Card.h"
#include "./components/Textbox.h"
#include <SFML/Audio.hpp>
#include <string>
#include <algorithm>

#define DELETE_KEY 8
#define ENTER_KEY 13
#define ESCAPE_KEY 27

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

    Font bPreplay;
    bPreplay.loadFromFile("./assets/fonts/BPreplay.otf");
    string path = "./assets/pokemon/info";

    int pokemonCount = countPokemon(path);

    //play background music
    SoundBuffer themeBuffer;
    themeBuffer.loadFromFile("./assets/music/theme.wav");
    Sound themeSound;
    themeSound.setBuffer(themeBuffer);
    themeSound.setVolume(1);
    themeSound.play();

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

    //selected option container
    sf::RectangleShape dropdownButton(sf::Vector2f(200, 50));
    dropdownButton.setFillColor(sf::Color::White);
    dropdownButton.setOutlineThickness(1);
    dropdownButton.setOutlineColor(sf::Color::Black);
    dropdownButton.setOrigin(dropdownButton.getGlobalBounds().width/2,dropdownButton.getGlobalBounds().height/2);
    dropdownButton.setPosition(525, 340);

    //selected option text
    sf::Text dropdownText(selectedOption, bPreplay, 30);
    dropdownText.setFillColor(sf::Color::Black);
    dropdownText.setOrigin(dropdownText.getGlobalBounds().width/2,dropdownText.getGlobalBounds().height/2);
    dropdownText.setPosition(dropdownButton.getPosition().x - (dropdownText.getGlobalBounds().width/2),dropdownButton.getPosition().y - (dropdownText.getGlobalBounds().height/2));

    //dropdown container
    sf::RectangleShape dropdownList;
    dropdownList.setFillColor(sf::Color::White);
    dropdownList.setOutlineThickness(1);
    dropdownList.setOutlineColor(sf::Color::Black);
    dropdownList.setOrigin(dropdownList.getGlobalBounds().width/2,dropdownList.getGlobalBounds().height/2);

    //search container
    RectangleShape textContainer;
    textContainer.setSize({300,50});
    textContainer.setFillColor(Color::White);
    textContainer.setOrigin(textContainer.getGlobalBounds().width/2,textContainer.getGlobalBounds().height/2);
    textContainer.setPosition(dropdownButton.getPosition().x + (textContainer.getGlobalBounds().width/2) - (dropdownButton.getGlobalBounds().width/2),200);
    
    //filter label text
    Text filterText("Type Filter:",bPreplay,30);
    filterText.setFillColor(Color::White);
    filterText.setOrigin(filterText.getGlobalBounds().height/2,filterText.getGlobalBounds().height/2);
    filterText.setPosition(200,dropdownText.getPosition().y);
    
    //search label text
    Text searchText("Search:",bPreplay,30);
    searchText.setFillColor(Color::White);
    searchText.setOrigin(0,searchText.getGlobalBounds().height/2);
    searchText.setPosition(200,textContainer.getPosition().y - (searchText.getGlobalBounds().height/2));

    string typeNames[] = {"Fire","Normal","Grass","Water","Ground",
                        "Rock","Bug","Electric","Ice","Fighting",
                        "Poison","Flying","Psychic","Dark","Dragon",
                        "Ghost","Steel","Fairy","All"};
    int typeCount = sizeof(typeNames)/sizeof(typeNames[0]);

    vector<Text> dropdownOptions;
    for(int i = 0; i < typeCount; i++){
        Text optionText(typeNames[i],bPreplay,30);
        optionText.setFillColor(Color::Black);
        optionText.setOrigin(optionText.getGlobalBounds().width/2,optionText.getGlobalBounds().height/2);
        dropdownOptions.push_back(optionText);
    }

    //current pokemon displayed
    string currentPokemon;

    //scene indicator
    int activeScene = 1;

    //indicator if a type filter from dropdown is selected
    bool isFilterActive = false;

    //vertical scroll counter
    int scrollCount = 0;

    bool isSearchEmpty = 1;

    Textbox textbox(20,Color::Black,false);
    textbox.setFont(bPreplay);
    textbox.setLimit(true,15);
    textbox.setOrigin(0,textbox.getGlobalBounds().height/2);
    textbox.setPosition({textContainer.getPosition().x - (textContainer.getGlobalBounds().width/2) + 50,textContainer.getPosition().y - (textbox.getGlobalBounds().height/2) - 12});
    
    
    while(window.isOpen()){
        Event event;
        View currentView = window.getView();
        
        if(activeScene == 1 || activeScene == 3){
            currentView.setCenter(window.getSize().x / 2, window.getSize().y / 2);
        }

        if(selectedOption == "All"){
            isFilterActive = false;
        }

        if(textbox.getText() == ""){
            isSearchEmpty = true;
        } else {
            isSearchEmpty = false;
        }

        currentView.setSize(window.getSize().x, window.getSize().y);
        window.setView(currentView);

        string** filteredPokemonList = new string*[pokemonCount];
        int filteredCount = 0;

        //filter pokemon per type
        if(isFilterActive){  
            if(isSearchEmpty){
                for (int i = 0; i < pokemonCount; ++i) {
                    if (createSubstring(pokemonList[i][2],',') == selectedOption) { 
                        filteredPokemonList[filteredCount] = new string[POKEMON_INFO_COUNT];
                        for (int j = 0; j < POKEMON_INFO_COUNT; ++j) {
                            filteredPokemonList[filteredCount][j] = pokemonList[i][j];
                        }
                        filteredCount++;
                    }
                }
            } else {
                for (int i = 0; i < pokemonCount; ++i) {
                    if (createSubstring(pokemonList[i][2],',') == selectedOption) { 
                        filteredPokemonList[filteredCount] = new string[POKEMON_INFO_COUNT];
                        if(isSubstringPresent(toLowercase(pokemonList[i][1]),textbox.getText())){
                            for (int j = 0; j < POKEMON_INFO_COUNT; ++j) {
                                filteredPokemonList[filteredCount][j] = pokemonList[i][j];
                            }
                            filteredCount++;
                        }
                    }
                }
            }
            numRows = (filteredCount + 3) / 4;
            numCols = 4;
        } else {
            if(isSearchEmpty){
                numRows = (pokemonCount + 3) / 4;
                numCols = 4;
            } else {
                for (int i = 0; i < pokemonCount; ++i) {
                    if (isSubstringPresent(toLowercase(pokemonList[i][1]),textbox.getText())) { 
                        filteredPokemonList[filteredCount] = new string[POKEMON_INFO_COUNT];
                            for (int j = 0; j < POKEMON_INFO_COUNT; ++j) {
                                filteredPokemonList[filteredCount][j] = pokemonList[i][j];
                            }
                            filteredCount++;
                    }
                }
            }
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

                        PreviewCard pcard(name, number, type, bPreplay);
                        pcard.setPosition(i, j);

                        PreviewCardList[i][j] = pcard;
                        listCounter++;
                    }
                } else {
                    if(isSearchEmpty){
                        if (listCounter < pokemonCount) {
                            string name = pokemonList[listCounter][1];
                            string number = pokemonList[listCounter][0];
                            string type = createSubstring(pokemonList[listCounter][2],',');

                            PreviewCard pcard(name,number,type,bPreplay);
                            pcard.setPosition(i,j);

                            PreviewCardList[i][j] = pcard;
                            listCounter++;
                        }
                    } else {
                        if (listCounter < filteredCount) {
                            string name = filteredPokemonList[listCounter][1];
                            string number = filteredPokemonList[listCounter][0];
                            string type = filteredPokemonList[listCounter][2];

                            PreviewCard pcard(name, number, type, bPreplay);
                            pcard.setPosition(i, j);

                            PreviewCardList[i][j] = pcard;
                            listCounter++;
                        }
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
                std::istringstream issEvo(pokemonList[i][4]);
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

        Card currentCard(Name, number, types, weaknesses, description, evolution, bPreplay);
    
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
                            textbox.setSelected(true);
                        } else {
                            textbox.setSelected(false);
                        }

                        //keyboard clicks
                        if(Keyboard::isKeyPressed(Keyboard::Return)){

                        }

                        //dropdown clicks
                        if (dropdownButton.getGlobalBounds().contains(mousePosition)) {
                        isDropdownOpen = !isDropdownOpen;
                        } else if (isDropdownOpen && dropdownList.getGlobalBounds().contains(mousePosition)) {
                            for(auto& option : dropdownOptions){
                                if(option.getGlobalBounds().contains(mousePosition)){
                                    selectedOption = option.getString();
                                    dropdownText.setString(selectedOption);
                                    if(selectedOption == "All"){
                                        isFilterActive = false;
                                    } else {
                                        isFilterActive = true;
                                    }
                                }
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

                        if(currentCard.getBgSprite().getGlobalBounds().contains(mousePosition)){
                            currentCard.play();
                        }
                    }
                }
            } else if (event.type == Event::MouseWheelScrolled) {
                //scene 2 scroll events
                if(activeScene == 2){
                    if (event.mouseWheelScroll.delta > 0){
                        if (view.getCenter().y - view.getSize().y / 2 > scrollLimitTop)
                            view.move(0, -150);
                            scrollCount -= 150;
                    } else {
                        if (view.getCenter().y + view.getSize().y / 2 < scrollLimitBottom)
                            view.move(0, 150);
                            scrollCount += 150;
                    }
                    window.setView(view);
                }
            } else if (event.type == Event::TextEntered){
                    textbox.typedOn(event);
            }
        }

        if (isDropdownOpen) {
            dropdownList.setSize(sf::Vector2f(dropdownButton.getSize().x, 580));
            dropdownList.setPosition(dropdownButton.getPosition().x-(dropdownList.getGlobalBounds().width/2),dropdownButton.getPosition().y + dropdownButton.getGlobalBounds().height);
            int optionCounter = 0;
            for(auto& optionText : dropdownOptions){
                optionText.setPosition(dropdownButton.getPosition().x, dropdownList.getPosition().y + 10 + 30*optionCounter);
                ++optionCounter;
            }
        }

        window.clear();

        if(activeScene == 1){
            window.draw(loadingDex);
        } else if(activeScene == 2){
            window.draw(dropdownButton);
            window.draw(dropdownText);
            window.draw(searchText);
            window.draw(textContainer);
            window.draw(filterText);
            textbox.drawTo(window);
            
            for(int i = 0; i < numRows; i++){
                for(int j = 0; j < numCols; j++){
                    PreviewCardList[i][j].drawTo(window);
                }
            }

            if (isDropdownOpen) {
                window.draw(dropdownList);
                for(auto& option : dropdownOptions){
                    window.draw(option);
                }   
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
