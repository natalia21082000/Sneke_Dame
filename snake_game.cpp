#include "snake_game.h"
#include <QDebug>
/*
QDebug - это класс в Qt, который предоставляет функции для отладки и вывода в консоль.
Он предоставляет простой и удобный способ вывода информации, такой как значения переменных,
структуры данных и сообщения об ошибках.
*/
#include <QPainter>
#include <QTime>
#include <QMessageBox>
#include <QImage>
#include <QPushButton>

snake_game::snake_game()
{
    //ЗАДАЕМ РАЗМЕР ПОЛЯ ПУТЕМ УМНОЖЕНИЯ ПРИВАТНЫХ ПОЛЕЙ КАССА snake_game
    this->resize(DOT_WIDTH*FIEILD_WIDTH, DOT_HEIGNT*FIELD_HEIGHT);
/*
Метод resize() в Qt используется для изменения размера виджета (игрового окна).
Он принимает два аргумента: ширину и высоту в пикселях.
*/
/*
• Гибкость: использование констант позволяет вам легко изменять размер игрового поля,
просто изменяя значения констант. Это удобно, если вы хотите,
чтобы игра поддерживала разные размеры поля.
• Масштабируемость: использование констант для определения размера игрового окна делает игру
более масштабируемой. Вы можете легко адаптировать размер игрового окна к разным разрешениям
экрана и соотношениям сторон.
• Согласованность: использование одних и тех же констант для определения размера игрового поля
и размера точек обеспечивает согласованность и единообразие в коде.

Кроме того, использование умножения констант вместо фиксированных значений позволяет вам
более четко определять размер игрового окна на основе размера точек и количества точек
по горизонтали и вертикали. Это делает код более понятным и простым в обслуживании.

*/

    //У ЭТОГО ОБЬЕКТА(snake_game) БУДЕТ ДАННЫЙ ЗАГОЛОВОК
    this->setWindowTitle("Snake game");
/*
Метод setWindowTitle() в Qt используется для установки заголовка окна для виджета(игрового окна).
*/
    //ИНИЦИАЛЕЗИРУЕМ ИГРУ
     initGame();

    //Создаем кнопку с текстом "Play"
    QPushButton *playButton = new QPushButton("Play", this);

    //создаем имя кнопки что бы потом ее скрыть
    playButton->setObjectName("playButton");

    // Устанавливаем размер и положение кнопки
    playButton->setGeometry(10, 370, 100, 30);

    // Подключаем сигнал нажатия кнопки к слоту для старта игры
    QObject::connect(playButton, &QPushButton::clicked, this, &snake_game::startGame);

    playButton->setStyleSheet("QPushButton { background-color: transparent; border: none; color: #333; font-size: 12px; }");


    QPushButton *pauseButton = new QPushButton("Pause", this);
    pauseButton->setObjectName("pauseButton");
    pauseButton->setGeometry(320, 370, 100, 30);
    QObject::connect(pauseButton, &QPushButton::clicked, this, &snake_game::pauseGame);

    pauseButton->setStyleSheet("QPushButton { background-color: transparent; border: none; color: #333; font-size: 12px; }");
}

void snake_game::timerEvent(QTimerEvent* e)
{
    Q_UNUSED(e);

    if(m_shouldStartGame && m_inGame)
    {
        check_apple();//СЬЕДАЕМ ЯБЛОКО
        move();//МЫ ДВИГАЕМ ВСЮ ЗМЕЙКУ
        check_fild();//ВЫХОД ЗА ПОЛЯ
        this->repaint();
/*
Когда вы вызываете repaint(), виджет добавляется в очередь перерисовки.
Qt обрабатывает очередь перерисовки в конце цикла обработки событий,
перерисовывая все виджеты в очереди.
*/
    }
}
// ОБРАБАТЫВАЕМ НАЖАТИЯ КЛАВИШИ
void snake_game::keyPressEvent(QKeyEvent* e)
{
    int key = e->key();
/*
 функция e->key()возвращает код нажатой клавиши.
*/
    if(key==Qt::Key_Left && m_dir !=Direcctions::right){m_dir=Direcctions::left; }
    if(key==Qt::Key_Right&& m_dir !=Direcctions::left ){m_dir=Direcctions::right;}
    if(key==Qt::Key_Up   && m_dir !=Direcctions::down ){m_dir=Direcctions::up;   }
    if(key==Qt::Key_Down && m_dir !=Direcctions::up   ){m_dir=Direcctions::down; }
/*
Прежде чем изменить направление движения змейки, код проверяет,
что змейка не движется в перпендикулярном направлении.
Это делается путем сравнения текущего направления движения змейки (m_dir) с направлением,
в которое змейка должна повернуть. Например, если змейка движется вверх или вниз,
она не может мгновенно повернуть налево или направо.
*/
/*
Пространство имен Qt содержит множество классов, функций и констант,
которые являются частью библиотеки Qt. Использование префикса Qt:: перед константами,
такими как Key_Left, гарантирует, что мы ссылаемся на правильную константу из пространства имен
Qt, а не на какую-либо другую константу с таким же именем в другом пространстве имен.
*/
}

void snake_game::paintEvent(QPaintEvent *event)
{
    // МЫ ГОВОРИМ ЧТО ПЕРЕМЕННАЯ event НЕ НУЖНА
    Q_UNUSED(event);
    /*
Q_UNUSED - это макрос в Qt, который используется для игнорирования
неиспользуемого параметра в функции.
*/
    doDrawing();
}

void snake_game::doDrawing()
{
    //НАШ ХОЛСТ QPainter
    QPainter qp(this);
    /*
QPainter - это класс в Qt, который предоставляет методы для рисования на виджетах
и других поверхностях рисования. Он предоставляет простой и удобный способ рисования примитивов,
таких как линии, прямоугольники, круги и текст, а также более сложных фигур и изображений.
*/

    // Устанавливаем цвет фона в черный
    qp.setBrush(Qt::black);
    // Рисуем прямоугольник заднего фона
    qp.drawRect(0, 0, width(), height());
/*
* 0, 0: Координаты левого верхнего угла прямоугольника.
* width(): Ширина прямоугольника, которая равна ширине виджета.
* height(): Высота прямоугольника, которая равна высоте виджета.
*/

    if(m_inGame)//ЕСЛИ МЫ В ИГРЕ
    {
        qp.setBrush(Qt::red);
        //РИСУЕМ КРУЖОЧЕК В РАНДОМНЫХ КООРДИНАТАХ С КОНСТАНТЫМИ ЗНАЧЕНИЯМИ
        qp.drawEllipse(m_apple.x()*DOT_WIDTH, m_apple.y()*DOT_HEIGNT, DOT_WIDTH,DOT_HEIGNT);
/*
Умножая m_apple.x() на DOT_WIDTH, мы преобразуем координату x из единиц сетки в единицы пикселей.
Это необходимо для правильного позиционирования кружка на экране.
Каждая ячейка сетки представляет определенное количество пикселей на экране,
а координаты яблока (m_apple.x(), m_apple.y()) указывают, в какой ячейке оно находится.
*/

        for(int i = 0;i<m_dots.size();i++)
        {
            if(i==0)//ЕСЛИ ГОЛОВА ТО
            {
                //РАССКРАШИВАЕМ ГОЛОВУ
                qp.setBrush(Qt::white);
                //РИСУЕМ КРУЖОЧЕК С КОНСТАНТЫМИ ЗНАЧЕНИЯМИ
                qp.drawEllipse(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGNT, DOT_WIDTH,DOT_HEIGNT);
            }
            else
            {
                //РАССКРАШИВАЕМ ВСЮ ОСТАВШЕЮСЯ ЗМЕЙКУ
                qp.setBrush(Qt::yellow);
                //РИСУЕМ КРУЖОЧЕК С КОНСТАНТЫМИ ЗНАЧЕНИЯМИ
                qp.drawEllipse(m_dots[i].x()*DOT_WIDTH,m_dots[i].y()*DOT_HEIGNT, DOT_WIDTH,DOT_HEIGNT);
            }
        }
    }
    else
    {
        gameOver();
    }
}
//РАНДОМНО РАСПОЛОГАЕМ ЯБЛОКО
void snake_game::localApple()
{
    QTime time = QTime::currentTime();//СОЗДАЕМ ОБЬЕКТ КОТОРЫЙ представляет текущее время суток.
    srand(static_cast<uint>(time.msec()));
/*
time.msec() возвращает количество миллисекунд,
static_cast<uint>(time.msec()) конвертирует количество миллисекунд в беззнаковое целое число.
*/

    bool intersects;
    do {
        m_apple.rx() = rand() % DOT_WIDTH;
        m_apple.ry() = rand() % DOT_HEIGNT;

        // Проверяем, не пересекается ли позиция яблока с змейкой
        intersects = false;
        for (const auto& dot : m_dots)
        {
            if (dot == m_apple)
            {
                intersects = true;
                break;
            }
        }
    } while (intersects);
}

void snake_game::move()
{
    //МЫ ДВИГАЕМ ВСЮ ЗМЕЙКУ
    for (int i=m_dots.size()-1; i>0; --i)//ДОБИРАЕМСЯ ДО ВТОРОЙ ТОЧКИ ТАК КАК ПЕРВАЯ ЭТО ГОЛОВА
    {
        m_dots[i]=m_dots[i-1];//СДВИГАЕМ ВСЕ ТОЧКИ
    }
    switch (m_dir)//А ЗДЕСЬ ГОЛОВА ПЕРЕМЕЩАЕТСЯ В УКАЗАННОМ НАПРАВЕЛЕНИИ
    {
    case left: {m_dots[0].rx()-=1;break;}
    case right:{m_dots[0].rx()+=1;break;}
    case up:   {m_dots[0].ry()-=1;break;}
    case down: {m_dots[0].ry()+=1;break;}
    }
}

void snake_game::check_fild()
{
    if(m_dots.size()>4)// КОГДА У НАС БОЛЬШЕ 4 ЭЛЕМЕНТОВ МЫ МОЖЕМ САМИ СЕБЯ УКУСИТЬ
    {
        for(int i = 1;i<m_dots.size();++i)//МЫ НЕ ВЛЮЧАЕМ ГОЛОВУ ПОЭТОМУ НАЧИНАЕМ С 1
        {
            if(m_dots[0]==m_dots[i]) //ЕСЛИ НАША ГОЛОВА РАВНА КАКОМУ ЛИБО ЭЛЕМЕНТУ
            {
                m_inGame =false;//ТО МЫ ЗАКАНЧИВАЕМ ИГРУ
            }
        }
    }
    //ЕСЛИ ЗМЕЙКА ВЫШЛА ЗА ПРЕДЕЛЫ ПОЛЯ
    if(m_dots[0].x()>=FIEILD_WIDTH){m_inGame=false;}//МЫ ЗАКАНЧИВАЕМ ИГРУ
    if(m_dots[0].x()<0)            {m_inGame=false;}//МЫ ЗАКАНЧИВАЕМ ИГРУ
    if(m_dots[0].y()>=FIELD_HEIGHT){m_inGame=false;}//МЫ ЗАКАНЧИВАЕМ ИГРУ
    if(m_dots[0].y()<0)            {m_inGame=false;}//МЫ ЗАКАНЧИВАЕМ ИГРУ

    if(!m_inGame)//ЕСЛИ МЫ НЕ В ИГРЕ
    {
        killTimer(m_timerId);//ОСТАНАВЛЕВАЕМ ТАЙМЕР
    }
}

void snake_game::gameOver()
{
    QMessageBox msgb;
    msgb.setText("Game Over");
    msgb.exec();
    initGame();
}

void snake_game::check_apple()
{
    if(m_apple==m_dots[0])//ЕСЛИ ЯБЛОКО РАВНО ГОЛОВЕ
    {
        m_dots.push_back((QPoint(0,0)));//МЫ ДОБОВЛЯЕМ РАЗМЕР К ЗМЕЙКЕ
        localApple();
    }
}

void snake_game::startGame()
{
    // После нажатия кнопки "Play" устанавливаем флаг, что игра должна начаться
    m_shouldStartGame = true;
    // Устанавливаем фокус на виджет игры после нажатия кнопки
    setFocus();

    // Скрыть кнопку "Play" после запуска игры
    QPushButton *playButton = this->findChild<QPushButton*>("playButton");
    if(playButton)
    {
        playButton->hide();
    }
}

void snake_game::pauseGame()
{
    m_shouldStartGame = !m_shouldStartGame; // Инвертируем флаг, чтобы поставить игру на паузу или возобновить её

    // Изменяем текст на кнопке в зависимости от состояния игры
    QPushButton *pauseButton = this->findChild<QPushButton*>("pauseButton");
    if(pauseButton)
    {
        if(m_shouldStartGame)
        {
            pauseButton->setText("Pause");
        }
        else
        {
            pauseButton->setText("Resume");
        }
    }
    // Устанавливаем фокус на виджет игры после нажатия кнопки
    setFocus();
}

void snake_game::initGame()
{
    m_inGame = true;//МЫ ГОВОРИМ ЧТО МЫ В ИГРЕ
    m_dir = right;// НАПРОВЛЕНИЯ НА ПРАВО
    m_dots.resize(3);//У НАС БУДЕТ 3 ЧАСТИ НАШЕЙ ЗМЕЙКИ
/*
resize задаем размер
*/
    //РАСПОЛОГАЕМ НАШУ ЗМЕЙКУ ТАК ЧТО БЫ ТРИ ЕЕ ЧАСТИ ШЛИ ДРУГ ЗА ДРУГОМ
    for(int i = 0;i<m_dots.size();i++)
    {
        m_dots[i].rx()=m_dots.size()-i-1;//m_dots БУДЕТ НАХОДИТСЯ КООРДИНАТЫ ГОЛОВЫ
        m_dots[i].ry()=0;//ВСЕ ЧАСТИ ЗМЕЙКИ БУДУТ НА ОДНОЙ СТРОКЕ ПО Y
    }
    localApple();//РАСПОЛОГАЕМ ЯБЛОКО
    m_timerId = startTimer(DELAY);//ОЬБЯВЛЯЕМ ТАЙМЕР ЧТО БЫ ОН ВЫЗЫВАЛ СЛОТ timerEvent
/*
Метод startTimer() запускает таймер в Qt.
Он принимает один аргумент: интервал в миллисекундах, с которым должен срабатывать таймер.
*/
}


