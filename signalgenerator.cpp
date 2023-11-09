#include "signalgenerator.h"
#include <cmath>
#include <fftw3.h>

SignalGenerator::SignalGenerator(QObject *parent)
    : QObject(parent)
{
    fftw_init_threads();
    fftPlan = fftw_plan_dft_1d(200, NULL, NULL, FFTW_FORWARD, FFTW_ESTIMATE);
}

double SignalGenerator::generateSinusValue(double time, double amplitude, double phase)
{
    return amplitude * std::sin(2 * M_PI * time + phase);
}

double SignalGenerator::generateLCMValue(double time, double frequencyDeviation, double centralFrequency)
{
    return std::sin(2 * M_PI * (centralFrequency * time + (frequencyDeviation / 2) * std::pow(time, 2)));
}

double SignalGenerator::generateTriangleWaveValue(double time, double amplitude)
{
    double period = 2.0;
    double t = std::fmod(time, period);
    double value = 2 * amplitude / period * (period / 2 - std::fabs(period / 2 - t));
    return value;
}

void SignalGenerator::calculateAmplitudeSpectrum(const QVector<double> &signal, QVector<double> &amplitudeSpectrum)
{
    int size = signal.size();

    fftw_complex* in = (fftw_complex*)fftw_malloc(size * sizeof(fftw_complex));
    fftw_complex* out = (fftw_complex*)fftw_malloc(size * sizeof(fftw_complex));

    for (int i = 0; i < size; i++) {
        in[i][0] = signal[i];
        in[i][1] = 0.0;
    }

    fftw_execute_dft(fftPlan, in, out);

    amplitudeSpectrum.resize(size);

    for (int i = 0; i < size; i++) {
        amplitudeSpectrum[i] = 2.0 * sqrt(out[i][0] * out[i][0] + out[i][1] * out[i][1]) / size;
    }

    fftw_free(in);
    fftw_free(out);
}

void SignalGenerator::calculatePhaseSpectrum(const QVector<double> &signal, QVector<double> &phaseSpectrum)
{
    int size = signal.size();

    fftw_complex* in = (fftw_complex*)fftw_malloc(size * sizeof(fftw_complex));
    fftw_complex* out = (fftw_complex*)fftw_malloc(size * sizeof(fftw_complex));

    for (int i = 0; i < size; i++) {
        in[i][0] = signal[i];
        in[i][1] = 0.0;
    }

    fftw_execute_dft(fftPlan, in, out);

    phaseSpectrum.resize(size);

    for (int i = 0; i < size; i++) {
        phaseSpectrum[i] = atan2(out[i][1], out[i][0]);
    }

    fftw_free(in);
    fftw_free(out);
}
