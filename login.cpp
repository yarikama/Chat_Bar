#include "header.h"

int main() {
    // 創建窗口
    sf::RenderWindow window(sf::VideoMode(500, 300), "Login Screen");
    window.setFramerateLimit(60);

    // 登入介面元素
    sf::Font font;
    if (!font.loadFromFile("Assets/Fonts/login_font.ttf")) {
        return -1;
    }

    sf::Text usernameLabel("Username:", font, 20);
    usernameLabel.setPosition(50, 50);
    sf::Text passwordLabel("Password:", font, 20);
    passwordLabel.setPosition(50, 120);

    sf::RectangleShape usernameBox(sf::Vector2f(300, 30));
    usernameBox.setPosition(150, 50);
    usernameBox.setFillColor(sf::Color::White);

    sf::RectangleShape passwordBox(sf::Vector2f(300, 30));
    passwordBox.setPosition(150, 120);
    passwordBox.setFillColor(sf::Color::White);

    sf::RectangleShape loginButton(sf::Vector2f(100, 40));
    loginButton.setPosition(200, 200);
    loginButton.setFillColor(sf::Color::Green);

    sf::Text loginButtonText("Login", font, 20);
    loginButtonText.setPosition(220, 205);
    loginButtonText.setFillColor(sf::Color::White);

    std::string username = "";
    std::string password = "";
    bool typingUsername = false, typingPassword = false;

    // 主循環
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // 處理鼠標點擊
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
                    if (usernameBox.getGlobalBounds().contains(mousePos)) {
                        typingUsername = true;
                        typingPassword = false;
                    } else if (passwordBox.getGlobalBounds().contains(mousePos)) {
                        typingPassword = true;
                        typingUsername = false;
                    } else if (loginButton.getGlobalBounds().contains(mousePos)) {
                        std::cout << "Username: " << username << "\nPassword: " << password << std::endl;

                        // 在這裡處理登入邏輯
                    }
                }
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    // Enter 鍵被按下
                    std::cout << "Username: " << username << "\nPassword: " << password << std::endl;
                    // 在這裡處理登入邏輯
                }
            }
            // 處理鍵盤輸入
            if (event.type == sf::Event::TextEntered) {
                if (typingUsername) {
                    if (event.text.unicode == '\b' && !username.empty()) { // Backspace
                        username.pop_back();
                    } else if (event.text.unicode < 128) {
                        username += static_cast<char>(event.text.unicode);
                    }
                } else if (typingPassword) {
                    if (event.text.unicode == '\b' && !password.empty()) { // Backspace
                        password.pop_back();
                    } else if (event.text.unicode < 128) {
                        password += static_cast<char>(event.text.unicode);
                    }
                }
            }
        }

        window.clear(sf::Color::Black);

        // 繪製介面元素
        window.draw(usernameLabel);
        window.draw(passwordLabel);
        window.draw(usernameBox);
        window.draw(passwordBox);
        window.draw(loginButton);
        window.draw(loginButtonText);

        // 在文本框中顯示已輸入的帳號和密碼（密碼以*顯示）
        sf::Text enteredUsername(username, font, 20);
        enteredUsername.setPosition(155, 55);
        enteredUsername.setFillColor(sf::Color::Black);
        window.draw(enteredUsername);

        std::string displayedPassword(password.length(), '*');
        sf::Text enteredPassword(displayedPassword, font, 20);
        enteredPassword.setPosition(155, 125);
        enteredPassword.setFillColor(sf::Color::Black);
        window.draw(enteredPassword);

        window.display();
    }

    return 0;
}