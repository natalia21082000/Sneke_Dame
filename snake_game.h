#ifndef SNAKE_GAME_H
/*
В Qt #ifndef SNAKE_GAME_H - это препроцессорная директива,
используемая для предотвращения повторного включения заголовочного файла
*/
#define SNAKE_GAME_H
/*
Директива #define используется для определения макрокоманды
*/

#include <QWidget>
/*
 * Виджеты - это исходные элементы для создания пользовательского интерфейса в Qt.
Виджеты могут отображать данные и информацию о состоянии, получить ввод от пользователя и
предоставлять контейнер для других виджетов, которые должны быть сгруппированы.
Виджет, не встроенный в родительский виджет, называется окном. Класс.
QWidget предоставляет базовую возможность для отрисовки на экране и для обработки событий
пользовательского ввода.
*/
#include <QVector>
/*
• Тип данных: QVector может хранить любые типы данных,
в то время как vector изначально предназначен для хранения примитивных типов данных,
таких как int и double. Для хранения объектов в vector необходимо использовать указатели или ссылки.
• Управление памятью: QVector автоматически управляет памятью для своих элементов,
 используя внутренний буфер. vector не управляет памятью, и вы должны вручную
выделять и освобождать память для своих элементов.
• Производительность: QVector обычно быстрее vector для небольших объемов данных,
поскольку он использует внутренний буфер для минимизации перераспределения памяти.
Однако vector может быть быстрее для больших объемов данных, поскольку он имеет
более эффективную реализацию управления памятью.
• Особенности Qt: QVector предоставляет дополнительные функции, специфичные для Qt,
такие как сигналы и слоты, которые позволяют легко связывать его с другими компонентами Qt.
*/
#include <QPoint>
/*
QPoint в Qt - это класс, представляющий точку в двумерном пространстве.
Он часто используется для хранения координат точек,
 размеров и положений виджетов и других графических элементов.

Класс QPoint имеет два открытых целочисленных члена: x и y, которые представляют координаты
точки по горизонтали и вертикали соответственно.
*/
#include <QKeyEvent>

class snake_game : public QWidget
{
    Q_OBJECT
/*
Q_OBJECT - это макрос в Qt, который используется для генерации метаданных для класса,
наследующего от QObject. Метаданные используются системой сигналов и слотов Qt для
автоматического подключения сигналов к слотам.
*/
public:
    snake_game();
protected:
 /*
Protected означает, что члены, объявленные как protected,
могут быть унаследованы и переопределены в подклассах,
но не могут быть напрямую доступны извне класса.
*/
    void timerEvent(QTimerEvent*) override;      // СЛОТ ИЗМЕНЕИЯ ВРЕМЕНИ
/*
QTimerEvent* - это указатель на объект QTimerEvent.
Объект QTimerEvent представляет событие таймера в Qt.
Он содержит информацию о том, какой таймер сработал и когда.
*/
/*
override для переопределения слота timerEvent,
который является виртуальным методом в базовом классе QObject.
Это позволяет предоставить собственную реализацию слота в  подклассе.
*/
    void keyPressEvent(QKeyEvent*) override;     // СЛОТ ИЗМЕНЕИЯ НАЖАТИЯ КНОПОК
    void paintEvent(QPaintEvent *event) override;// ФУНКЦИЯ ОТРЕСОВКИ
private:
/*
Статические переменные используются в классах C++ для хранения данных,
которые должны сохраняться в течение всего времени существования класса,
даже если нет экземпляров класса.

Статические переменные также могут использоваться для хранения констант,
которые должны быть доступны всем экземплярам класса.
*/
    static const int DOT_WIDTH    = 20; // РАЗМЕР ПОЛЯ
    static const int DOT_HEIGNT   = 20; //РАЗМЕР ПОЛЯ
    static const int FIEILD_WIDTH = 20; //УВЕЛИЧИВАЕТ РАЗМЕРЫ ЗМЕЙКИ
    static const int FIELD_HEIGHT = 20; //УВЕЛИЧИВАЕТ РАЗМЕРЫ ЗМЕЙКИ
    static const int DELAY        = 200;//СКОРОСТЬ ЗМЕЙКИ

    void doDrawing();  //РИСУЕМ ЯБЛОКО И ЗМЕЙКУ
    void localApple(); //РАНДОМНО РАСПОЛОГАЕМ ЯБЛОКО
    void move();       //ДВИЖЕНИЯ ЗМЕЙКИ
    void check_fild(); //ФУНКЦИЯ ПРОВЕРЯЕТ НЕ УШЛА ЛИ ЗМЕЙКА ЗА ПОЛЯ
    void gameOver();   // В СЛУЧАИ ПРОЙГРЫША ВЫВОДИМ НА ЭКРАН Game over
    void check_apple();//ПРОВЕРЯЕТ СКУШАЛА ЛИ ЗМЕЙКА ЯБЛОЧКО
    void startGame();  // нажимаем на плей и игра запускается
    void pauseGame();  // кнопка паузы
    void initGame();   // РАСПОЛОГАЕМ НАШУ ЗМЕЙКУ И ЯБЛОКО НА КАРТЕ

    int  m_timerId;         //СОЗДАЕМ ПЕРЕМЕНУЮ В КОТОРОЙ БУДЕМ ХРАНИТЬ ВРЕМЯ
    bool m_inGame;         //СОЗДАЕМ ПЕРЕМЕНУЮ ДЛЯ ПРОВЕРОК ЕСЛИ ЗМЕЙКА ВРЕЖЕТСЯ В СТЕНКУ ИЛИ СЬЕСТ САМА СЕБЯ
    bool m_shouldStartGame;//Добавляем флаг, указывающий на то, что игра должна стартовать только после нажатия кнопки "Play"

    QPoint m_apple;                      //ИНФОРМАЦИЯ О ЯБЛОКЕ ХРАНИТ КООРДИНАТЫ X Y
    enum Direcctions{left,right,up,down};//ПЕРЕЧЕСЛЕНИЯ НАШИХ ДВИЖЕНИЙ
    Direcctions m_dir;
    QVector<QPoint> m_dots;              //ИНФОРМАЦИЯ О НАШЕЙ ЗМЕЙКЕ
};

#endif // SNAKE_GAME_H