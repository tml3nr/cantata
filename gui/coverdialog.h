/*
 * Cantata
 *
 * Copyright (c) 2011-2012 Craig Drummond <craig.p.drummond@gmail.com>
 *
 * ----
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#ifndef COVER_DIALOG_H
#define COVER_DIALOG_H

#include "config.h"
#include "dialog.h"
#include "song.h"
#include "ui_coverdialog.h"
#include <QtCore/QSet>
#include <QtCore/QList>

class QNetworkReply;
class QUrl;
class QTemporaryFile;
class QListWidgetItem;
class QLabel;
class QProgressBar;
class QScrollArea;
class QWheelEvent;
class CoverItem;
class ExistingCover;
class Spinner;

class CoverPreview : public Dialog
{
    Q_OBJECT

public:
    CoverPreview(QWidget *p);
    virtual ~CoverPreview() { }
    void showImage(const QImage &img, const QString &u);
    void downloading(const QString &u);
    bool aboutToShow(const QString &u) const { return u==url; }

private Q_SLOTS:
    void progress(qint64 rx, qint64 total);

private:
    void scaleImage(int adjust);
    void wheelEvent(QWheelEvent *event);

private:
    QString url;
    QLabel *loadingLabel;
    QProgressBar *pbar;
    QLabel *imageLabel;
    QScrollArea *scrollArea;
    double zoom;
    int imgW;
    int imgH;
};

class CoverDialog : public Dialog, public Ui::CoverDialog
{
    Q_OBJECT

public:
    static int instanceCount();

    enum DownloadType {
        DL_Query,
        DL_Thumbnail,
        DL_LargePreview,
        DL_LargeSave
    };

    enum FileType {
        FT_Jpg,
        FT_Png,
        FT_Other
    };

    CoverDialog(QWidget *parent);
    virtual ~CoverDialog();

    void show(const Song &s);
    int imageSize() const { return iSize; }

private Q_SLOTS:
    void queryJobFinished();
    void downloadJobFinished();
    void showImage(QListWidgetItem *item);
    void sendQuery();
    void checkStatus();

private:
    CoverPreview *previewDialog();
    void insertItem(CoverItem *item);
    QNetworkReply * downloadImage(const QString &url, DownloadType dlType);
    void cancelQuery();
    void clearTempFiles();
    void sendQueryRequest(const QUrl &url);
    void parseLstFmQueryResponse(const QString &resp);
    void parseGoogleQueryResponse(const QString &resp);
//    void parseYahooQueryResponse(const QString &resp);
//    void parseDiscogsQueryResponse(const QString &resp);
    void slotButtonClicked(int button);
    bool saveCover(const QString &src, const QImage &img);
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);

private:
    Song song;
    ExistingCover *existing;
    QString currentQueryString;
    QSet<QNetworkReply *> currentQuery;
    QSet<QString> currentUrls;
    QSet<QString> currentDroppedFiles;
    QList<QTemporaryFile *> tempFiles;
    CoverPreview *preview;
    bool saving;
    int iSize;
    Spinner *spinner;
    int page;
};

#endif
