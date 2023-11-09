#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <QObject>
#include <vector>
#include <complex>
#include <fftw3.h>

class SignalGenerator : public QObject
{
    Q_OBJECT

public:
    explicit SignalGenerator(QObject *parent = nullptr);

    double generateSinusValue(double time, double amplitude, double phase);
    double generateLCMValue(double time, double frequencyDeviation, double centralFrequency);
    double generateTriangleWaveValue(double time, double amplitude);

    void calculateAmplitudeSpectrum(const QVector<double> &signal, QVector<double> &amplitudeSpectrum);
    void calculatePhaseSpectrum(const QVector<double> &signal, QVector<double> &phaseSpectrum);

private:
    fftw_plan fftPlan;
};

#endif // SIGNALGENERATOR_H
