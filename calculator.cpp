
#include <QtWidgets>
#include <QProcess>
#include <math.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <string>
#include "button.h"
#include "calculator.h"
#include <vector>
#include <stdexcept>

using namespace std;
double result = 0.0;

QString evaluate;
Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
{
    /*!
     * \brief Intialisaation
     */
    sumInMemory = 0.0;
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = true;

    display = new QLineEdit("");
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(30);

    display1 = new QLineEdit("");
    display1->setReadOnly(true);
    display1->setAlignment(Qt::AlignRight);
    display1->setMaxLength(60);

    display2 = new QLineEdit("");
    display2->setReadOnly(true);
    display2->setAlignment(Qt::AlignRight);
    display2->setMaxLength(30);

    QFont font = display->font();
    font.setPointSize(font.pointSize() +1);
    QFont font1 = display1->font();
    font1.setPointSize(font.pointSize() + 1);
    display->setFont(font);
    display1->setFont(font1);
    display2->setFont(font);

    for (int i = 0; i < NumDigitButtons; ++i)
    {
        digitButtons[i] = createButton(QString::number(i), SLOT(digitClicked()));
    }
    /*!
     * \brief buttons for decimal
     */
    Button *pointButton = createButton(tr("."), SLOT(pointClicked()));

    /*!
     * \brief buttons for backspace, clear and clear all
     */
    Button *backspaceButton = createButton(tr("Backspace"), SLOT(backspaceClicked()));
    Button *clearAllButton = createButton(tr("Clear All"), SLOT(clearAll()));

    /*!
     * \brief simple functions
     * 1) division
     * 2) times
     * 3) minus
     * 4) plus
     * 5) remainder
     */
    Button *divisionButton = createButton(tr("/"), SLOT(unaryOperatorClicked()));
    Button *timesButton = createButton(tr("*"), SLOT(unaryOperatorClicked()));
    Button *minusButton = createButton(tr("-"), SLOT(unaryOperatorClicked()));
    Button *plusButton = createButton(tr("+"), SLOT(unaryOperatorClicked()));
    Button *remainderButton = createButton(tr("%"), SLOT(unaryOperatorClicked()));

    /*!
     * \brief advance functions
     * 1) sqaureroot
     * 2) power
     * 3) equal
     */
    Button *squareRootButton = createButton(tr("Sqrt"), SLOT(unaryOperatorClicked()));
    Button *powerButton = createButton(tr("x^y"), SLOT(unaryOperatorClicked()));
    Button *equalButton = createButton(tr("="), SLOT(equalClicked()));

    /*!
     * \brief trignometric functions
     * 1) sin
     * 2) cos
     * 3) tan
     */
    Button *sinButton = createButton(tr("sin"), SLOT(unaryOperatorClicked()));
    Button *cosButton = createButton(tr("cos"), SLOT(unaryOperatorClicked()));

    /*!
     * \brief logrithmic
     * 1) log to the base 10
     * 2) log to base e
     */
    Button *logButton = createButton(tr("log"), SLOT(unaryOperatorClicked()));
    Button *lnButton = createButton(tr("ln"), SLOT(unaryOperatorClicked()));

    /*!
     * \brief brackets
     */
    Button *rightbracketButton = createButton(tr(")"), SLOT(unaryOperatorClicked()));
    Button *leftbracketButton = createButton(tr("("), SLOT(unaryOperatorClicked()));

    /*!
     * \brief random functions
     */
    Button *exponentialButton = createButton(tr("e^x"), SLOT(unaryOperatorClicked()));

    /*!
     * \brief exit
     */
    QPushButton *quitButton = new QPushButton(tr("Quit"));

    /*!
     * \brief writeButton
     * convert digits to words
     */
    Button *writeButton = createButton(tr("write"), SLOT(writeClicked()));


    QGridLayout *mainLayout = new QGridLayout;
#if defined(Q_OS_SYMBIAN) || defined(Q_WS_MAEMO_5) || defined(Q_WS_SIMULATOR)
    mainLayout->setSizeConstraint(QLayout::SetNoConstraint);
#else
    mainLayout->setSizeConstraint(QLayout::SetFixedSize);
#endif

    mainLayout->addWidget(display, 0, 0, 1, 15);
    mainLayout->addWidget(backspaceButton, 1, 0, 1, 3);
    mainLayout->addWidget(clearAllButton, 1, 3, 1, 3);
    mainLayout->addWidget(quitButton, 1, 6, 1, 1);
    QObject::connect(quitButton, SIGNAL(clicked()), this, SLOT(quit()));

    /*!
     * Display the numbers
     */
    for (int i = 1; i < NumDigitButtons; ++i)
    {
        int row = ((9 - i) / 3) + 2;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget(digitButtons[i], row, column);
    }

    /*!
     * Display of all the funtions on calculator gui
     */
    mainLayout->addWidget(digitButtons[0], 5, 1);
    mainLayout->addWidget(pointButton, 5, 2);
    mainLayout->addWidget(remainderButton, 5, 3);

    mainLayout->addWidget(divisionButton, 2, 4);
    mainLayout->addWidget(timesButton, 3, 4);
    mainLayout->addWidget(minusButton, 4, 4);
    mainLayout->addWidget(plusButton, 5, 4);

    mainLayout->addWidget(squareRootButton, 2, 5);
    mainLayout->addWidget(powerButton, 3, 5);
    mainLayout->addWidget(lnButton, 4, 5);
    mainLayout->addWidget(equalButton, 5, 5);

    mainLayout->addWidget(sinButton, 2, 0);
    mainLayout->addWidget(cosButton, 3, 0);
    mainLayout->addWidget(logButton, 4, 0);

    mainLayout->addWidget(rightbracketButton, 2, 6);
    mainLayout->addWidget(leftbracketButton, 3, 6);
    mainLayout->addWidget(exponentialButton, 4, 6);
    mainLayout->addWidget(writeButton, 5, 6);
    mainLayout->addWidget(display1, 6, 0, 5, 30);
    mainLayout->addWidget(display2, 11, 0, 1, 30);
    setLayout(mainLayout);

    setWindowTitle(tr("Calculator"));
}

/*!
 * \brief Calculator::digitClicked
 * digit is pressed and printed
 */
void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    int digitValue = clickedButton->text().toInt();
    if (display->text() == "0" && digitValue == 0.0)
        return;
    display->setText(display->text() + QString::number(digitValue));
    evaluate=evaluate+QString::number(digitValue);
}

/*!
 * \brief Calculator::unaryOperatorClicked
 * contains all the operations
 */
void Calculator::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->text();

    QString tempx ;
    QString tempy ;
    QString tempi = display->text();


    if (clickedOperator == tr("Sqrt"))
    {
        tempx = "sqrt";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("x^y"))
    {
        tempx = "^";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("sin"))
    {
        tempx = "sin";
        display->setText(tempi+tempx);
        tempy= "s";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("cos"))
    {
        tempx = "cos";
        display->setText(tempi+tempx);
        tempy = "c";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("tan"))
    {
        tempx = "tan";
        display->setText(tempi+tempx);
        tempy = "t";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("e^x"))
    {
        tempx = "e^";
        display->setText(tempi+tempx);
        tempy = "e";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("ln"))
    {
        tempx = "ln";
        display->setText(tempi+tempx);
        tempy="l";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("log"))
    {
        tempx = "log";
        display->setText(tempi+tempx);
        tempy="1/l(10)*l";
        evaluate = evaluate+tempy;
    }
    else if (clickedOperator == tr("+"))
    {
        tempx = "+";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("-"))
    {
        tempx = "-";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("*"))
    {
        tempx = "*";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("/"))
    {
        tempx = "/";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("%"))
    {
        tempx = "%";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr("("))
    {
        tempx = "(";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    else if (clickedOperator == tr(")"))
    {
        tempx = ")";
        display->setText(tempi+tempx);
        evaluate = evaluate+tempx;
    }
    waitingForOperand = true;
}
/*!
 * \brief Calculator::equalClicked
 * on pressing equal button , value of whole expression will be calculated
 */
void Calculator::equalClicked()
{
    /*!
     * bash script is used to calculate the value of expression
     * bc is used here
     */
    display1->setText("");
    display2->setText("");
    QProcess *proc = new QProcess();
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->start(tr("/home/dushyant/calculator/cl.bash"),QStringList() << evaluate);
    QByteArray data;
    while(proc->waitForReadyRead())
        data.append(proc->readAll());


    // Output the data
    qDebug(data.data());
    qDebug("Done!");
    display2->setText(data.data());
    QString random = display2->text();
    result = random.toDouble();
    result = ceil(result*100000)/100000;
    display2->setText("");
    display2->setText(QString::number(result));

    sumSoFar = 0.0;
    waitingForOperand = true;
}

/*!
 * \brief Calculator::HelperConvertNumberToText it is a helper function for conversion of integer to word
 * \param num integer
 * \param buf buffer
 * \param len length of buffer
 * \return word
 */
bool Calculator::HelperConvertNumberToText(int num, char *buf, int len)
{
    static char *strones[] = {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight","Nine", "Ten", "Eleven", "Twelve", "Thirteen", "Fourteen","Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    static char *strtens[] = {"Ten", "Twenty", "Thirty", "Fourty", "Fifty", "Sixty","Seventy", "Eighty", "Ninety", "Hundred"};
    char result[1024];
    int single, tens, hundreds;
    if(num > 1000)
        return false;
    hundreds = num / 100;
    num = num - hundreds * 100;
    if( num < 20)
    {
        tens = 0; // special case
        single = num;
    }
    else
    {
        tens = num / 10;
        num = num - tens * 10;
        single = num;
    }
    memset(result, 0, 1024);
    if(hundreds > 0)
    {
        strcat(result, strones[hundreds-1]);
        strcat(result, " Hundred ");
    }
    if(tens > 0)
    {
        strcat(result, strtens[tens-1]);
        strcat(result, " ");
    }
    if(single > 0)
    {
        strcat(result, strones[single-1]);
        strcat(result, " ");
    }
    if(len > strlen(result))
        strcpy(buf, result);
    return true;
}

/*!
 * \brief Calculator::ConvertNumberToText
 * \param num input
 * \param buf buffer for storage
 * \param length of buffer
 * \return it will return the integer to word converted string
 */
bool Calculator::ConvertNumberToText(int num, char *buf, int len)
{
    char tres[1024];
    char result[1024];
    int thousands;
    int temp;
    if(num < 0 || num > 100000)
    {
        printf( " %d \t Not Supported\n", num);
        return false;
    }
    if( num == 0)
    {
        printf(" %d \t Zero\n", num);
        return false;
    }
    memset(result, 0, 1024);
    if(num < 1000)
    {
        HelperConvertNumberToText(num, (char*) &tres, 1024);
        strcat(result, tres);
    }
    else
    {
        thousands = num / 1000;
        temp = num - thousands * 1000;
        HelperConvertNumberToText(thousands, (char*) &tres, 1024);
        strcat(result, tres);
        strcat(result, "Thousand ");
        HelperConvertNumberToText(temp, (char*) &tres, 1024);
        strcat(result, tres);
    }
    if(len > strlen(result))
        strcpy(buf, result);
    return true;
}

/*!
 * \brief Calculator::tobedisplayed
 * \param x
 * \return it will return the word for each digit
 */
string Calculator::tobedisplayed(int x)
{
    if(x==0)
    {
        return "zero";
    }
    else if(x==1)
    {
        return "one";
    }
    else if(x==2)
    {
        return "two";
    }
    else if(x==3)
    {
        return "three";
    }
    else if(x==4)
    {
        return "four";
    }
    else if(x==5)
    {
        return "five";
    }
    else if(x==6)
    {
        return "six";
    }
    else if(x==7)
    {
        return "seven";
    }
    else if(x==8)
    {
        return "eight";
    }
    else if(x==9)
    {
        return "nine";
    }
}

/*!
 * \brief Calculator::writeClicked
 * clicking on write button will print the output
 * and text to speech will happen
 */
void Calculator::writeClicked()
{
    /*!
     * decalring the parameters
     */
    double param, fractpart, intpart;
    param = result;
    fractpart = modf (param , &intpart);
    int len = 1024;
    char result1[1024];
    string displayed = "";
    /*!
     * converting numbers to words
     */
    if(param<0)
    {
        displayed=displayed+"negative";
        param=-param;
    }
    if (ConvertNumberToText(intpart, result1, len)== true)
        displayed=displayed+result1;
    if(fractpart!=0)
        displayed = displayed + "point";
    int count=0;
    /*!
     * converting decimal to words
     */
    while(fractpart!=0 && count<=5)
    {
        count=count+1;
        fractpart=10*fractpart;

        int zx=(int(fractpart));
        displayed=displayed+tobedisplayed(zx);
        fractpart=fractpart-double(int(zx));
    }
    /*!
     * \brief calling bash
     * bash is called which uses google text to speech converter
     */
    QString myString = QString::fromStdString(displayed);
    display1->setText(myString);
    QProcess *proc = new QProcess();
    proc->setProcessChannelMode(QProcess::MergedChannels);
    proc->start(tr("/home/dushyant/calculator/random.bash"),QStringList() << myString);
    sumSoFar = 0.0;
    waitingForOperand = true;
}

/*!
 * \brief Calculator::pointClicked
 * include a decimal
 */
void Calculator::pointClicked()
{
    display->setText(display->text() + tr("."));
    evaluate=evaluate+".";

}

/*!
 * \brief Calculator::backspaceClicked
 * delete a digit or operation that is printed on screen ( similar to backspace )
 */
void Calculator::backspaceClicked()
{
    QString text = display->text();
    text.chop(1);
    if (text.isEmpty())
    {
        text = "0";
        waitingForOperand = true;
    }
    display->setText(text);
    evaluate.chop(1);
}

/*!
 * \brief Calculator::clearAll
 * clear everything and every data
 */
void Calculator::clearAll()
{
    display->setText("");
    display1->setText("");
    display2->setText("");
    waitingForOperand = true;
    result =0;
    evaluate ="";
}

/*!
 * \brief Calculator::createButton
 * \param text
 * \param member
 * create the button
 */
Button *Calculator::createButton(const QString &text, const char *member)
{
    Button *button = new Button(text);
    connect(button, SIGNAL(clicked()), this, member);
    return button;
}

/*!
 * \brief Calculator::abortOperation
 * abort operation
 */
void Calculator::abortOperation()
{
    clearAll();
    display->setText(tr("####"));
}

/*!
 * \brief Calculator::quit
 * quit button in calculator
 */
void Calculator::quit()
{
    QMessageBox messageBox;
    messageBox.setWindowTitle(tr("Calculator"));
    messageBox.setText(tr("Do you really want to quit?"));
    messageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    messageBox.setDefaultButton(QMessageBox::No);
    if (messageBox.exec() == QMessageBox::Yes)
        qApp->quit();
}
