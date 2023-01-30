/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtcamera.h"
#include <QApplication>
#include <QMediaService>
#include <QMediaRecorder>
#include <QCameraViewfinder>
#include <QCameraInfo>
#include <QMediaMetaData>
#include <QMessageBox>
#include <QPalette>
#include <QTabWidget>
#include <QtWidgets>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "guiutil.h"

#include <qzxing.h>

#define FONT_SIZE 12
#define QCAMERA_CAPTURE_MODE "Image Mode"
#define QCAMERA_VIDEO_MODE "Video Mode"
#define DIR_USERDATA "/userdata"
#define DIR_HOME QStandardPaths::writableLocation(QStandardPaths::HomeLocation)
Q_DECLARE_METATYPE(QCameraInfo)

qtCamera::qtCamera()
{
    //setWindowFlags(Qt::FramelessWindowHint);
    resize(640, 480);

    setWindowTitle("Scan QR Code");

    initlayout();

    scanTimer = new QTimer(this);
    connect(scanTimer,	SIGNAL(timeout()), this, SLOT(scanQRCode()));
    scanTimer->start(500);

    imageCnt = videoCnt = 0;
    qrCode = "";

    setCamera(QCameraInfo::defaultCamera());
}

void qtCamera::initlayout()
{   
    QBoxLayout *vLayout = new QVBoxLayout();
    vLayout->setContentsMargins(0, 0, 0, 0);
    vLayout->setAlignment(Qt::AlignTop);
    viewfinder.setWindowFlag(Qt::FramelessWindowHint);
    viewfinder.setGeometry(0, 0, 640, 480);
    viewfinder.show();
    vLayout->addWidget(&viewfinder);

    setLayout(vLayout);

    lblQRcode = new QLabel(this);
    lblQRcode->setGeometry(20, 10, 500, 60);
    lblQRcode->setStyleSheet(QString("background-color: transparent;border: 0px;"));
    lblQRcode->setText("<font color=\"red\">Detected QR Code:</font>");
    lblQRcode->hide();

    QFont font = GUIUtil::bitcoinAddressFont();
    font.setBold(true);
    lblQRcode->setFont(font);
}

void qtCamera::setCamera(const QCameraInfo &cameraInfo)
{
    m_camera.reset(new QCamera(cameraInfo));

    connect(m_camera.data(), QOverload<QCamera::Error>::of(&QCamera::error), this, &qtCamera::displayCameraError);

    m_imageCapture.reset(new QCameraImageCapture(m_camera.data()));

    configureCaptureSettings();

    m_camera->setViewfinder(&viewfinder);

    connect(m_imageCapture.data(), &QCameraImageCapture::imageCaptured, this, &qtCamera::imageCaptured);
    connect(m_imageCapture.data(), &QCameraImageCapture::imageSaved, this, &qtCamera::imageSaved);
    connect(m_imageCapture.data(), QOverload<int, QCameraImageCapture::Error, const QString &>::of(&QCameraImageCapture::error),
            this, &qtCamera::displayCaptureError);

    updateCaptureMode();
}

void qtCamera::configureCaptureSettings()
{
    QSize size(640, 480);

    m_imageSettings.setCodec("jpeg");
    m_imageSettings.setQuality(QMultimedia::VeryHighQuality);
    m_imageSettings.setResolution(size);
    m_imageCapture->setEncodingSettings(m_imageSettings);

    m_audioSettings.setCodec("audio/x-adpcm");
    m_audioSettings.setChannelCount(2);
    m_audioSettings.setQuality(QMultimedia::NormalQuality);

    m_videoSettings.setCodec("video/x-h264");
    m_videoSettings.setResolution(size);
    m_videoSettings.setQuality(QMultimedia::NormalQuality);
}

void qtCamera::updateCaptureMode()
{
    m_camera->unload();
    m_camera->setCaptureMode(QCamera::CaptureStillImage);
    m_camera->start();
}

void qtCamera::displayCaptureError(int id, const QCameraImageCapture::Error error, const QString &errorString)
{
    Q_UNUSED(id);
    Q_UNUSED(error);
    QMessageBox::warning(this, tr("Image Capture Error"), errorString);
    m_isCapturingImage = false;
}

void qtCamera::displayCameraError()
{
    QMessageBox::warning(this, tr("Camera Error"), m_camera->errorString());
}

void qtCamera::imageSaved(int id, const QString &fileName)
{
    QFile file (fileName);
    file.remove();

    Q_UNUSED(id);
    m_isCapturingImage = false;
    if (m_applicationExiting)
        close();
}

void qtCamera::closeEvent(QCloseEvent *event)
{
    /*
    if (m_isCapturingImage) {
        setEnabled(false);
        m_applicationExiting = true;
        event->ignore();
    }
    else */
    {
        scanTimer->stop();
        event->accept();
    }
}

void qtCamera::onCameraCapture()
{
    if(m_imageCapture->isReadyForCapture())
        takeImage();
}

void qtCamera::takeImage()
{
    m_isCapturingImage = true;
    m_imageCapture->capture("");
}

void qtCamera::scanQRCode()
{
    onCameraCapture();
}

void qtCamera::imageCaptured(int id, const QImage &preview)
{
    QZXing zxing;
    QString decode = zxing.decodeImage(preview);
    if (!decode.isEmpty()) {
        lblQRcode->show();
        QString text = "<font color=\"red\" font-size=\"12px\">Detected QR Code: ";
        text += decode;
        qrCode = decode;
        qrCode = qrCode.replace("chesscoin:", "");
        text += "</font>";
        lblQRcode->setText(text);
        accept();
    }
}

