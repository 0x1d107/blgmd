title: Inverting a Modified Short-time Fourier Transform
date: 2023-09-10
tags: math

# Inverting a Modified Short-Time Fourier Transform
In digital signal processing it can be useful to boost frequencies in the signal at
a certain time. For example, it can be used to compensate for the diispation of sound in the medium.

To know what frequencies are present in the signal mathematicians usually use the *Fourier transform*. 

$$\hat{f}(\omega) = \int_{-\infty}^{\infty} f(x) e^{-2\pi i}\, dx$$

Or its discrete variant

$$\hat{f}[k] = \sum_{n=0}^{N-1} f[k] e^{\frac{-2\pi i k n }{ N}}$$

But the *Fourier transform* only tells you what frequencies are present in the entire signal.

## STFT
To know what frequencies are present in a short time frame, the signal is split into regions using
a *window function* and then the Fourier transform is applied. 
This transformation is called *short-time Fourier Transform*(STFT) and the tranform of a discrete signal is called STFT matrix. 

$$M[k, l] = \frac{1}{L} \sum_{m=1}^K x_l[m] e^{-i 2 \pi m k / K},$$

$$x_l[m] = x[m + l H] w[m].$$

Where:

* $L$ is the window length
* $H$ is the step size (window length minus overlap)
* $K$ is the number of samples (DFT size)
* $k = 1 \dots K$  is the frequency sample index
* $l$ is the window index
* $m$ is the time index since the start of the window
* $w[m]$ is the window function

Here is a python STFT implementation using numpy. Please note the padding the signal to the nearest integer number of windows.

```py
def stft(signal, nperseg, noverlap):
    window = np.hanning(nperseg)
    window_step = nperseg - noverlap
    right_padding = window_step * int(len(signal) / window_step + 1) - len(signal)
    signal= np.pad(signal, (window_step, right_padding), 'constant', constant_values=(0, 0))
    windows = [signal[i:i+nperseg] * window for i in range(0, len(signal) - window_step, window_step)]
    spectrogram = [np.fft.rfft(win) for win in windows]
    return np.array(spectrogram).T
```

## ISTFT
The STFT is invertable, so the signal can be
reconstructed from its STFT matrix. This allows us to modify frequencies of the signal at a certain time window.
But not every STFT matrix has a real corresponding signal. A naive implementation of the inverse STFT of a modified STFT matrix can result in discontinuity at the windows' borders.

The estimation of the signal obtained by minimizing the error is decribed in the [IEEE paper by Griffin et al.](https://ieeexplore.ieee.org/document/1164317) 

$$x(n) = \frac{\sum_{m=-\infty}^{\infty}y(m,n)w(m - n)}{\sum_{m=-\infty}^{\infty}w^2(m-n)}$$

Here's a simple python implementation of the algorithm:

```py
def istft(spectogram, nperseg, noverlap):
    window = np.hanning(nperseg)
    window_step = nperseg - noverlap
    signal_len = (spectogram.shape[1] + 1) * window_step
    x = np.zeros(signal_len) 
    norm = np.zeros(signal_len) 
    for n, i  in enumerate(range(0, signal_len - window_step, window_step)):
        x[i:i+nperseg] += np.real(np.fft.irfft(spectogram[:, n])) * window
        norm[i:i+nperseg] += window * window
    x /= np.where(norm > 1e-10, norm, 1.0)
    return x[window_step:]
```
