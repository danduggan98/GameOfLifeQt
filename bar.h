#ifndef BAR_H
#define BAR_H

#include <QObject>

class Bar : public QObject {
    Q_OBJECT

    private:
        float width_;
        float height_;

    public:
        explicit Bar(float width, float height, QObject *parent = nullptr);
        float get_width() { return width_; };
        float get_height() { return height_; };
};

#endif // BAR_H
