////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <stack>
#include <queue>
#include <tuple>
#include <cmath>

#define SUPPORT_TEXTURE 0

const double MIN_LEN = 20.f;
const double MIN_FACTOR = .80f;
const double WAIT_TIME = 1.0/60;

template<class T>
inline T Clip(T curV, T minV, T maxV)
{
    return std::max(minV, std::min(curV, maxV));
}

class LineDraw
{
private:
    sf::Sprite m_sprite;
    sf::RenderWindow& m_window;
    sf::RenderTexture& m_texture;
    bool isDrawFinished;
    void draw(const sf::Drawable &target)
    {
#if SUPPORT_TEXTURE
        m_texture.draw(target);
        m_texture.display();

        m_window.clear(sf::Color::White);
        m_window.draw(m_sprite);
        m_window.display();
#else
        m_window.draw(target);
        m_window.display();
        sf::sleep(sf::seconds(WAIT_TIME));
#endif
    }
    void drawLinesRecur(
        const sf::Vector2f &sPos,
        double len, double angle,
        const sf::Color &color,
        int max_X = 800, int max_Y = 600)
    {
        if (len < MIN_LEN)
        {
            return;
        }

        sf::Vector2f endPos;
        endPos.x = Clip<float>(sPos.x + len * std::cos(angle + M_PI / 4), 0.f, max_X);
        endPos.y = Clip<float>(sPos.y - len * std::sin(angle + M_PI / 4), 0.f, max_Y);

        sf::VertexArray lineToDraw(sf::Lines, 2);
        lineToDraw[0].position = sPos;
        lineToDraw[1].position = endPos;
        lineToDraw[1].color = lineToDraw[0].color = color;
        draw(lineToDraw);


        sf::sleep(sf::seconds(WAIT_TIME));
        drawLinesRecur(endPos, MIN_FACTOR*len, angle + M_PI / 4, color, max_X, max_Y);

        endPos.x = Clip<float>(sPos.x + len * std::cos(angle - M_PI / 4), 0.f, max_X);
        endPos.y = Clip<float>(sPos.y - len * std::sin(angle - M_PI / 4), 0.f, max_Y);

        lineToDraw[1].position = endPos;
        lineToDraw[1].color = lineToDraw[0].color = color;
        draw(lineToDraw);

        sf::sleep(sf::seconds(WAIT_TIME));
        drawLinesRecur(endPos, MIN_FACTOR*len, angle - M_PI / 4, color, max_X, max_Y);
    }

public:

    LineDraw(sf::RenderWindow& window, sf::RenderTexture& texture):
        m_window(window), m_texture(texture), isDrawFinished(false)
    {
        m_sprite.setTexture(m_texture.getTexture());
    }
    
    void drawLinesDFSRecur(
        const sf::Vector2f &sPos,
        double len, double angle,
        const sf::Color &color,
        int max_X = 800, int max_Y = 600)
    {
        if (!isDrawFinished)
        {
            drawLinesRecur(sPos, len, angle, color, max_X, max_Y);
            isDrawFinished = true;
        }
        else
        {
            m_window.draw(m_sprite);
        }
    }


    void drawLinesBFS(
        const sf::Vector2f &sPos,
        double len, double angle,
        const sf::Color &color,
        int max_X = 800, int max_Y = 600)
    {
        if (!isDrawFinished)
        {
            using ParaType = std::tuple < sf::Vector2f, double, double > ;
            std::queue<ParaType> lineQueue;
            sf::VertexArray lineToDraw(sf::Lines, 2);

            lineQueue.push(std::make_tuple(sPos, len, angle + M_PI / 4.f));
            lineQueue.push(std::make_tuple(sPos, len, angle - M_PI / 4.f));

            sf::Vector2f endPos;
            while (!lineQueue.empty())
            {
                auto curPara = lineQueue.front();  lineQueue.pop();
                sf::Vector2f startPos = std::get<0>(curPara);
                double curLen = std::get<1>(curPara);
                double curAngle = std::get<2>(curPara);

                endPos.x = Clip<float>(startPos.x + curLen * std::cos(curAngle), 0.f, max_X);
                endPos.y = Clip<float>(startPos.y - curLen * std::sin(curAngle), 0.f, max_Y);


                lineToDraw[0].position = startPos;
                lineToDraw[1].position = endPos;
                lineToDraw[1].color = lineToDraw[0].color = color;
                draw(lineToDraw);
                sf::sleep(sf::seconds(WAIT_TIME));

                double nextLen = MIN_FACTOR * curLen;
                if (nextLen >= MIN_LEN)
                {
                    lineQueue.push(std::make_tuple(endPos, nextLen, curAngle + M_PI / 4.f));
                    lineQueue.push(std::make_tuple(endPos, nextLen, curAngle - M_PI / 4.f));
                }

            }
            isDrawFinished = true;
        }
        else
        {
            m_window.draw(m_sprite);
        }

    }

    //@brief   must push invalid node to Stack
    //         to make the stack work normally
    void drawLinesDFS(
        const sf::Vector2f &sPos,
        double len, double angle,
        const sf::Color &color,
        int max_X = 800, int max_Y = 600)
    {
        if (!isDrawFinished)
        {
            using ParaType = std::tuple < sf::Vector2f, double, double > ;
            std::stack<ParaType> lineStack;
            sf::VertexArray lineToDraw(sf::Lines, 2);

            lineStack.push(std::make_tuple(sPos, len, angle));
            sf::Vector2f startPos;
            sf::Vector2f endPos;
            double curLen;
            double curAngle;

            while (!lineStack.empty())
            {
                while (true)
                {
                    // 1. Pre-Order draw left line
                    auto topPara = lineStack.top();
                    startPos = std::get<0>(topPara);
                    curLen = std::get<1>(topPara);
                    curAngle = std::get<2>(topPara);
                    if (curLen < MIN_LEN) // use for right child backtrack
                    {
                        break;
                    }
                    endPos.x = Clip<float>(startPos.x + curLen * std::cos(curAngle + M_PI / 4), 0.f, max_X);
                    endPos.y = Clip<float>(startPos.y - curLen * std::sin(curAngle + M_PI / 4), 0.f, max_Y);
                    lineToDraw[0].position = startPos;
                    lineToDraw[1].position = endPos;
                    lineToDraw[1].color = lineToDraw[0].color = color;
                    draw(lineToDraw);

                    // 2. Push left line end point to stack

                    lineStack.push(std::make_tuple(endPos, MIN_FACTOR * curLen, curAngle + M_PI / 4));

                }

                // 3. Pop Invalid top node
                lineStack.pop();

                if (!lineStack.empty())
                {
                    // 4. In-Order draw right line
                    auto topPara = lineStack.top(); lineStack.pop();
                    startPos = std::get<0>(topPara);
                    curLen = std::get<1>(topPara);
                    curAngle = std::get<2>(topPara);
                    endPos.x = Clip<float>(startPos.x + curLen * std::cos(curAngle - M_PI / 4), 0.f, max_X);
                    endPos.y = Clip<float>(startPos.y - curLen * std::sin(curAngle - M_PI / 4), 0.f, max_Y);
                    lineToDraw[0].position = startPos;
                    lineToDraw[1].position = endPos;
                    lineToDraw[1].color = lineToDraw[0].color = color;
                    draw(lineToDraw);

                    lineStack.push(std::make_tuple(endPos, MIN_FACTOR * curLen, curAngle - M_PI / 4));
                }
            }

            isDrawFinished = true;
        }
        else
        {
            m_window.draw(m_sprite);
        }
    }

};




int main()
{
    unsigned int windowWidth = 800, windowHeight = 600;
    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), L"ÍøÒ×±ÊÊÔµÝ¹é»æÍ¼!");

    sf::RenderTexture texture;
    if (!texture.create(windowWidth, windowHeight))
        return -1;
    texture.clear(sf::Color::White);
    bool isFirst = true;
    sf::Vector2f sPos(windowWidth / 2, windowHeight*3.f / 4);
    LineDraw lineDraw(window, texture);


    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
        }

        window.clear(sf::Color::White);

        // lineDraw.drawLinesBFS(
        //     sPos,
        //     100.f, M_PI / 2, sf::Color::Red, windowWidth, windowHeight);

        // lineDraw.drawLinesDFSRecur(
        //     sPos,
        //     100.f, M_PI / 2, sf::Color::Red, windowWidth, windowHeight);

        lineDraw.drawLinesDFS(
            sPos,
            100.f, M_PI / 2, sf::Color::Red, windowWidth, windowHeight);

        window.display();
    }



}