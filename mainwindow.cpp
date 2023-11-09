#include "mainwindow.h"
#include "signalgenerator.h"
#include <QtCharts>
#include <QRadioButton>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    signalGenerator = new SignalGenerator(this);

    generateButton = new QPushButton("Generate", this);
    amplitudeLabel = new QLabel("Amplitude:", this);
    amplitudeSpinBox = new QSpinBox(this);
    phaseLabel = new QLabel("Phase:", this);
    phaseSpinBox = new QSpinBox(this);
    sinusRadio = new QRadioButton("Sinus", this);
    lcmRadio = new QRadioButton("Linear Chirp Modulation (LCM)", this);
    triangleWaveRadio = new QRadioButton("Triangle Wave", this);
    amplitudeSpectrumCheckbox = new QCheckBox("Amplitude Spectrum", this);
    phaseSpectrumCheckbox = new QCheckBox("Phase Spectrum", this);
    frequencyDeviationLabel = new QLabel("Frequency Deviation:", this);
    frequencyDeviationSpinBox = new QSpinBox(this);
    centralFrequencyLabel = new QLabel("Central Frequency:", this);
    centralFrequencySpinBox = new QSpinBox(this);
    QChart *chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    series = new QLineSeries();
    chart->addSeries(series);

    QValueAxis *axisX = new QValueAxis;
    QValueAxis *axisY = new QValueAxis;
    axisX->setLabelFormat("%.2f");
    axisX->setTitleText("Time");
    axisY->setLabelFormat("%.2f");
    axisY->setTitleText("Amplitude");
    chart->setAxisX(axisX, series);
    chart->setAxisY(axisY, series);

    QChart *amplitudeSpectrumChart = new QChart();
    amplitudeSpectrumView = new QChartView(amplitudeSpectrumChart);
    amplitudeSpectrumSeries = new QLineSeries();
    amplitudeSpectrumChart->addSeries(amplitudeSpectrumSeries);

    QValueAxis *amplitudeSpectrumAxisX = new QValueAxis;
    QValueAxis *amplitudeSpectrumAxisY = new QValueAxis;
    amplitudeSpectrumAxisX->setLabelFormat("%.2f");
    amplitudeSpectrumAxisX->setTitleText("Frequency");
    amplitudeSpectrumAxisY->setLabelFormat("%.2f");
    amplitudeSpectrumAxisY->setTitleText("Amplitude");
    amplitudeSpectrumChart->setAxisX(amplitudeSpectrumAxisX, amplitudeSpectrumSeries);
    amplitudeSpectrumChart->setAxisY(amplitudeSpectrumAxisY, amplitudeSpectrumSeries);

    QChart *phaseSpectrumChart = new QChart();
    phaseSpectrumView = new QChartView(phaseSpectrumChart);
    phaseSpectrumSeries = new QLineSeries();
    phaseSpectrumChart->addSeries(phaseSpectrumSeries);

    QValueAxis *phaseSpectrumAxisX = new QValueAxis;
    QValueAxis *phaseSpectrumAxisY = new QValueAxis;
    phaseSpectrumAxisX->setLabelFormat("%.2f");
    phaseSpectrumAxisX->setTitleText("Frequency");
    phaseSpectrumAxisY->setLabelFormat("%.2f");
    phaseSpectrumAxisY->setTitleText("Phase");
    phaseSpectrumChart->setAxisX(phaseSpectrumAxisX, phaseSpectrumSeries);
    phaseSpectrumChart->setAxisY(phaseSpectrumAxisY, phaseSpectrumSeries);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    QVBoxLayout *leftLayout = new QVBoxLayout();
    QVBoxLayout *rightLayout = new QVBoxLayout();

    leftLayout->addWidget(generateButton);
    leftLayout->addWidget(sinusRadio);
    leftLayout->addWidget(lcmRadio);
    leftLayout->addWidget(triangleWaveRadio);
    leftLayout->addWidget(amplitudeLabel);
    leftLayout->addWidget(amplitudeSpinBox);
    leftLayout->addWidget(phaseLabel);
    leftLayout->addWidget(phaseSpinBox);
    leftLayout->addWidget(frequencyDeviationLabel);
    leftLayout->addWidget(frequencyDeviationSpinBox);
    leftLayout->addWidget(centralFrequencyLabel);
    leftLayout->addWidget(centralFrequencySpinBox);

    rightLayout->addWidget(chartView);
    rightLayout->addWidget(amplitudeSpectrumView);
    rightLayout->addWidget(phaseSpectrumView);
    rightLayout->addWidget(amplitudeSpectrumCheckbox);
    rightLayout->addWidget(phaseSpectrumCheckbox);

    mainLayout->addLayout(leftLayout);
    mainLayout->addLayout(rightLayout);

    setCentralWidget(new QWidget(this));
    centralWidget()->setLayout(mainLayout);

    connect(generateButton, &QPushButton::clicked, this, &MainWindow::onGenerateClicked);
    connect(sinusRadio, &QRadioButton::toggled, this, &MainWindow::onRadioToggled);
    connect(lcmRadio, &QRadioButton::toggled, this, &MainWindow::onRadioToggled);
    connect(triangleWaveRadio, &QRadioButton::toggled, this, &MainWindow::onRadioToggled);

    // По умолчанию выбирается синусоида
    sinusRadio->setChecked(true);
}



MainWindow::~MainWindow()
{
}

void MainWindow::onRadioToggled()
{
    amplitudeLabel->setEnabled(sinusRadio->isChecked() || triangleWaveRadio->isChecked());
    amplitudeSpinBox->setEnabled(sinusRadio->isChecked() || triangleWaveRadio->isChecked());
    phaseLabel->setEnabled(sinusRadio->isChecked());
    phaseSpinBox->setEnabled(sinusRadio->isChecked());
    frequencyDeviationLabel->setEnabled(lcmRadio->isChecked());
    frequencyDeviationSpinBox->setEnabled(lcmRadio->isChecked());
    centralFrequencyLabel->setEnabled(lcmRadio->isChecked());
    centralFrequencySpinBox->setEnabled(lcmRadio->isChecked());
}

void MainWindow::onGenerateClicked()
{
    series->clear();
    amplitudeSpectrumSeries->clear();
    phaseSpectrumSeries->clear();

    if (sinusRadio->isChecked()) {
        double amplitude = amplitudeSpinBox->value();
        double phase = phaseSpinBox->value();

        QVector<double> signalData;
        for (double time = 0.0; time <= 10.0; time += 0.05)
        {
            double value = signalGenerator->generateSinusValue(time, amplitude, phase);
            series->append(time, value);
            signalData.append(value);
        }

        QVector<double> amplitudeSpectrum;
        QVector<double> phaseSpectrum;
        signalGenerator->calculateAmplitudeSpectrum(signalData, amplitudeSpectrum);
        signalGenerator->calculatePhaseSpectrum(signalData, phaseSpectrum);

        // Заполняем графики амплитудного и фазового спектров
        for (int i = 0; i < amplitudeSpectrum.size(); i++) {
            amplitudeSpectrumSeries->append(i * 0.1, amplitudeSpectrum[i]);
            phaseSpectrumSeries->append(i * 0.1, phaseSpectrum[i]);
        }
    } else if (lcmRadio->isChecked()) {
        double frequencyDeviation = frequencyDeviationSpinBox->value();
        double centralFrequency = centralFrequencySpinBox->value();


        QVector<double> signalData;
        for (double time = 0.0; time <= 10.0; time += 0.05)
        {
            double value = signalGenerator->generateLCMValue(time, frequencyDeviation, centralFrequency);
            series->append(time, value);
            signalData.append(value);
        }

        // Расчет и отображение амплитудного и фазового спектров
        QVector<double> amplitudeSpectrum;
        QVector<double> phaseSpectrum;
        signalGenerator->calculateAmplitudeSpectrum(signalData, amplitudeSpectrum);
        signalGenerator->calculatePhaseSpectrum(signalData, phaseSpectrum);

        // Заполняем графики амплитудного и фазового спектров
        for (int i = 0; i < amplitudeSpectrum.size(); i++) {
            amplitudeSpectrumSeries->append(i * 0.1, amplitudeSpectrum[i]);
            phaseSpectrumSeries->append(i * 0.1, phaseSpectrum[i]);
        }
    } else if (triangleWaveRadio->isChecked()) {
        double amplitude = amplitudeSpinBox->value();


        QVector<double> signalData;
        for (double time = 0.0; time <= 10.0; time += 0.05)
        {
            double value = signalGenerator->generateTriangleWaveValue(time, amplitude);
            series->append(time, value);
            signalData.append(value);
        }

        // Расчет и отображение амплитудного и фазового спектров
        QVector<double> amplitudeSpectrum;
        QVector<double> phaseSpectrum;
        signalGenerator->calculateAmplitudeSpectrum(signalData, amplitudeSpectrum);
        signalGenerator->calculatePhaseSpectrum(signalData, phaseSpectrum);

        // Заполняем графики амплитудного и фазового спектров
        for (int i = 0; i < amplitudeSpectrum.size(); i++) {
            amplitudeSpectrumSeries->append(i * 0.1, amplitudeSpectrum[i]);
            phaseSpectrumSeries->append(i * 0.1, phaseSpectrum[i]);
        }
    }

    // Обновляем графики сигнала и спектров
    chartView->chart()->removeSeries(series);
    chartView->chart()->addSeries(series);

    amplitudeSpectrumView->chart()->removeSeries(amplitudeSpectrumSeries); // Убираем старый амплитудный спектр
    amplitudeSpectrumView->chart()->addSeries(amplitudeSpectrumSeries); // Добавляем новый амплитудный спектр

    phaseSpectrumView->chart()->removeSeries(phaseSpectrumSeries); // Убираем старый фазовый спектр
    phaseSpectrumView->chart()->addSeries(phaseSpectrumSeries); // Добавляем новый фазовый спектр
}

