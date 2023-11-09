#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class SignalGenerator;
class QPushButton;
class QSpinBox;
class QChartView;
class QChart;
class QValueAxis;
class QLineSeries;
class QLabel;
class QRadioButton;
class QCheckBox;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void onGenerateClicked();
    void onRadioToggled();

private:
    SignalGenerator *signalGenerator;
    QPushButton *generateButton;
    QLabel *amplitudeLabel;
    QSpinBox *amplitudeSpinBox;
    QLabel *phaseLabel;
    QSpinBox *phaseSpinBox;
    QRadioButton *sinusRadio;
    QRadioButton *lcmRadio;
    QRadioButton *triangleWaveRadio;
    QCheckBox *amplitudeSpectrumCheckbox;
    QCheckBox *phaseSpectrumCheckbox;
    QLabel *frequencyDeviationLabel;
    QSpinBox *frequencyDeviationSpinBox;
    QLabel *centralFrequencyLabel;
    QSpinBox *centralFrequencySpinBox;
    QChartView *chartView;
    QLineSeries *series;
    QChartView *amplitudeSpectrumView;
    QLineSeries *amplitudeSpectrumSeries;
    QChartView *phaseSpectrumView;
    QLineSeries *phaseSpectrumSeries;
};

#endif
