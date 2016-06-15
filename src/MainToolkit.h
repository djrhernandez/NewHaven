// Created by Junior on 6/14/2016.
#ifndef MAINTOOLKIT_H
#define MAINTOOLKIT_H


class MainToolkit
{
public:
    MainToolkit(int argc = 0, char **argv = 0);
    virtual ~MainToolkit();

    enum Key { KeyEnter, KeySpace, KeyTab, KeyEscape,
        KeyUp, KeyDown, KeyLeft, KeyRight };
    enum Button { Left, Middle, Right };

    virtual bool run() = 0;
    virtual bool checkErrors() = 0;
    virtual void setVideoMode() = 0;
    virtual void grabMouse(bool) = 0;
};

#endif //MAINTOOLKIT_H