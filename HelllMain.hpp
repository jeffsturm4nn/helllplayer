#include <iostream>
#include <windows.h>
#include <process.h>
#include <math.h>

#include <QtGui>

#include <id3.h>
#include <id3/tag.h>
#include <id3/misc_support.h>

#include <sigar.h>
#include <sigar_format.h>
#include <sigar_fileinfo.h>

#include <irrKlang.h>

#include "ui_Helll.h"

#define keyhit(x)     GetAsyncKeyState(x)

using namespace irrklang;
using namespace std;
using namespace dami;
using namespace Qt;

namespace HELLL{

int msgbox(QString caption, QString text, UINT flags)
{
    return MessageBox(NULL, text.toAscii().constData(), caption.toAscii().constData(), flags);
}

class HelllMain : public QMainWindow, private Ui::MainWindow
{

    Q_OBJECT

private:
    ISoundEngine *SE;
    ISoundSource *file;
    ISound *song;
    ISoundEffectControl *effc;

    QTimer *timer;
    QTimer *keyloop;
    QTimer *sys;

    sigar_t *sigar;
    sigar_proc_cpu_t proc;

private slots:
    void updateTime();
    void getSysInfo();
    void loadFiles(void);
    void setPlayPause();
    void setStop();
    void setVolume();
    void setPBSpeed();
    void setSliderPos();
    void setPlayPos();

    void enableWReverb();
    void setWReverbOnOff();
    void enableEcho();
    void setEchoOnOff();
    void enableFlanger();
    void setFlangerOnOff();
    void enableGargle();
    void setGargleOnOff();
    void enableCompressor();
    void setCompOnOff();
    void enableDistort();
    void setDistortOnOff();
    void enableChorus();
    void setChorusOnOff();

    void disableAllEffects();

    void addTrack(QString);
    void deleteSelected();
    void setTrack(QListWidgetItem*);
    bool nextTrack();
    bool previousTrack();
    void checkKeyboard();

    void loadPList();
    void savePList();

public:
HelllMain();

void normalize(void);
void emptyBuffers(void);
void enablePlayFunc(bool);
void initSong(QString);
bool initSongFromArg(int argc, char* argv[]);
//void initSongFromSource(ISoundSource *&source);

~HelllMain()
{
    SE->removeAllSoundSources();
    SE->drop();
    sigar_close(sigar);

    free(timer); timer = NULL;
    free(keyloop); keyloop = NULL;
    free(sys); sys = NULL;
}

};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


HelllMain::HelllMain()
{
//---------------------------------------------HELLL UI QT4 WIDGETS PARAMETERS--------------------------------------------------
    setupUi(this);

    setFixedSize(862,654);
    setWindowIcon(QIcon(tr("fire.ico")));

    sysInfo2->hide();

    timer = new QTimer;
    timer->setInterval(5);

    keyloop = new QTimer;
    keyloop->setInterval(120);
    keyloop->start();

    sys = new QTimer;
    sys->setInterval(620);
    sys->start();

    sigar_open(&sigar);

    //Initialize IrrKlang Engine Vars
    SE = createIrrKlangDevice();
    file = NULL;
    song = NULL;
    effc = NULL;

//--------------------------------------CONNECTIONS---------------------------------------------
    connect(playButton, SIGNAL(clicked()), this, SLOT(setPlayPause())); //PLAY BUTTON CONNECTION
    connect(stopButton, SIGNAL(clicked()), this, SLOT(setStop()));  //STOP BUTTON CONNECTION

    connect(volumeSlider, SIGNAL(valueChanged(int)), this, SLOT(setVolume())); //VOLUME SLIDER CONNECTION
    connect(pbspeedSlider, SIGNAL(valueChanged(int)), this, SLOT(setPBSpeed())); //PLAYBACK SPEED SLIDER CONNECTION
    connect(playposSlider, SIGNAL(sliderMoved(int)), this, SLOT(setSliderPos())); //PLAY POSITION SLIDER CONNECTION #1
    connect(playposSlider, SIGNAL(sliderReleased()), this, SLOT(setPlayPos()));  //PLAY POSITION SLIDER CONNECTION #2

    connect(wreverbGain, SIGNAL(valueChanged(int)), this, SLOT(enableWReverb()));  //WAVES REVERB SOUND EFFECT BUTTON CONNECTIONS
    connect(wreverbMix, SIGNAL(valueChanged(int)), this, SLOT(enableWReverb()));
    connect(wreverbTime, SIGNAL(valueChanged(int)), this, SLOT(enableWReverb()));
    connect(wreverbHFTimeRatio, SIGNAL(valueChanged(int)), this, SLOT(enableWReverb()));
    connect(wreverbOnOff, SIGNAL(clicked()), this, SLOT(setWReverbOnOff()));

    connect(echoWDMix, SIGNAL(valueChanged(int)), this, SLOT(enableEcho())); //ECHO SOUND EFFECT BUTTON CONNECTIONS
    connect(echoFDBack, SIGNAL(valueChanged(int)), this, SLOT(enableEcho()));
    connect(echoLDelay, SIGNAL(valueChanged(int)), this, SLOT(enableEcho()));
    connect(echoRDelay, SIGNAL(valueChanged(int)), this, SLOT(enableEcho()));
    connect(echoPanDelay, SIGNAL(valueChanged(int)), this, SLOT(enableEcho()));
    connect(echoOnOff, SIGNAL(clicked()), this, SLOT(setEchoOnOff()));

    connect(flangerWDMix,  SIGNAL(valueChanged(int)), this, SLOT(enableFlanger())); //FLANGER SOUND EFFECT BUTTON CONNECTIONS
    connect(flangerDepth,  SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerFDBack, SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerFreq,   SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerTWForm, SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerDelay,  SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerPhase,  SIGNAL(valueChanged(int)), this, SLOT(enableFlanger()));
    connect(flangerOnOff, SIGNAL(clicked()), this, SLOT(setFlangerOnOff()));

    connect(gargleRateHZ, SIGNAL(valueChanged(int)), this, SLOT(enableGargle())); //GARGLE SOUND EFFECT BUTTON CONNECTION
    connect(gargleSWForm, SIGNAL(valueChanged(int)), this, SLOT(enableGargle()));
    connect(gargleOnOff, SIGNAL(clicked()), this, SLOT(setGargleOnOff()));

    connect(compOnOff, SIGNAL(clicked()), this, SLOT(setCompOnOff())); //COMPRESSOR SOUND EFFECT BUTTON CONNECTION
    connect(compGain, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));
    connect(compAttack, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));
    connect(compRelease, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));
    connect(compThreshold, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));
    connect(compRatio, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));
    connect(compPreDelay, SIGNAL(valueChanged(int)), this, SLOT(enableCompressor()));

    connect(distOnOff, SIGNAL(clicked()), this, SLOT(setDistortOnOff())); //DISTORTION SOUND EFFECT BUTTON CONNECTION
    connect(distGain, SIGNAL(valueChanged(int)), this, SLOT(enableDistort()));
    connect( distEdge, SIGNAL(valueChanged(int)), this, SLOT(enableDistort()));
    connect( distPEQCFreq, SIGNAL(valueChanged(int)), this, SLOT(enableDistort()));
    connect( distPEQBandw, SIGNAL(valueChanged(int)), this, SLOT(enableDistort()));
    connect( distPreLPCutOff, SIGNAL(valueChanged(int)), this, SLOT(enableDistort()));

    connect(chorusOnOff, SIGNAL(clicked()), this, SLOT(setChorusOnOff())); //CHORUS SOUND EFFECT BUTTON CONNECTION
    connect( chorusWDMix, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));
    connect( chorusDepth, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));
    connect( chorusFDBack, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));
    connect( chorusFreq, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));
    connect( chorusSWForm, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));
    connect( chorusDelay, SIGNAL(valueChanged(int)), this, SLOT(enableChorus()));

    connect(disableAllButton, SIGNAL(clicked()), this, SLOT(disableAllEffects())); //DISABLE ALL EFFECTS BUTTON CONNECTION

    connect(addFilesButton, SIGNAL(clicked()), this, SLOT(loadFiles())); //ADD FILES BUTTON CONNECTION
    connect(deleteTracks, SIGNAL(clicked()), this, SLOT(deleteSelected())); //DELETE SELECTED BUTTON CONNECTION
    connect(nextButton, SIGNAL(clicked()), this, SLOT(nextTrack())); //NEXT TRACK BUTTON CONNECTION
    connect(previousButton, SIGNAL(clicked()), this, SLOT(previousTrack())); //PREVIOUS TRACK BUTTON CONNECTION

    connect(playList, SIGNAL(itemDoubleClicked(QListWidgetItem*)), this, SLOT(setTrack(QListWidgetItem*)));
    //connect(playList, SIGNAL(), this, SLOT());
    connect(loadPlayList, SIGNAL(clicked()), this, SLOT(loadPList()));
    connect(savePlayList, SIGNAL(clicked()), this, SLOT(savePList()));

    connect(keyloop, SIGNAL(timeout()), this, SLOT(checkKeyboard())); //SHORTCUT CONTROLS UPDATE CONNECTION
    connect(timer, SIGNAL(timeout()), this, SLOT(updateTime())); //PLAY TIME UPDATE CONNECTION
    connect(sys, SIGNAL(timeout()), this, SLOT(getSysInfo()));    //SYSTEM INFORMATION CONNECTION
}

//-------------------------------------------------------------------------------
void HelllMain::setPlayPause()
{
        if(!song->getIsPaused())
            {song->setIsPaused(true);}
        else
            {song->setIsPaused(false);}

        if(song->isFinished())
            {initSong(file->getName());}
}

//-------------------------------------------------------------------------------
void HelllMain::setStop()
{
       if(not song->isFinished())
            song->setIsPaused(true);
            song->setPlayPosition(0);
}

//-------------------------------------------------------------------------------
void HelllMain::setVolume()
{
    int value = volumeSlider->value();
    song->setVolume(value/100.0f);
    volumeLCD->display(value);
}

//-------------------------------------------------------------------------------
void HelllMain::setPBSpeed()
{
    int value = pbspeedSlider->value();
    song->setPlaybackSpeed(value/100.0f);
    pbspeedLCD->display(value);
}

//-------------------------------------------------------------------------------
void HelllMain::setSliderPos()
{
    timeBar->setValue(playposSlider->value());
}

//-------------------------------------------------------------------------------
void HelllMain::setPlayPos()
{
    song->setPlayPosition(playposSlider->value());
}

//-------------------------------------------------------------------------------
void HelllMain::emptyBuffers(void)
{
    SE->removeAllSoundSources();
}

//-------------------------------------------------------------------------------
void HelllMain::normalize(void)
{
    if(not keepPBSpeed->isChecked()){ pbspeedSlider->setSliderPosition(100); }
    if(not keepVolume->isChecked()) { volumeSlider->setSliderPosition(65);   }
    if(keepEffectsOn->isChecked())
    {
        if(wreverbOnOff->text() == "ON"){ setWReverbOnOff(); }
        if(echoOnOff->text() == "ON")   { setEchoOnOff();    }
        if(flangerOnOff->text() == "ON"){ setFlangerOnOff(); }
        if(gargleOnOff->text() == "ON") { setGargleOnOff();  }
        if(compOnOff->text() == "ON")   { setCompOnOff();    }
        if(distOnOff->text() == "ON")   { setDistortOnOff(); }
        if(chorusOnOff->text() == "ON") { setChorusOnOff();  }
    }
    else
    {
        disableAllEffects();
    }

    timeBar->setValue(0);
    playposSlider->setSliderPosition(0);

    clockMin->display("0");
    clockSec->display("0");

}

//-------------------------------------------------------------------------------
void HelllMain::disableAllEffects()
{
        if(not song->isFinished())
        {
            effc->disableAllEffects();

            wreverbOnOff->setText("OFF");
            echoOnOff->setText("OFF");
            flangerOnOff->setText("OFF");
            gargleOnOff->setText("OFF");
            compOnOff->setText("OFF");
            chorusOnOff->setText("OFF");
            distOnOff->setText("OFF");
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableWReverb()
{
        if(not song->isFinished())
        {
            if(effc->isWavesReverbSoundEffectEnabled())

            effc->enableWavesReverbSoundEffect(wreverbGain->value(), wreverbMix->value()/10,
                                               wreverbTime->value()/1000, wreverbHFTimeRatio->value()/1000);
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableEcho()
{
        if(not song->isFinished())
        {
            if(effc->isEchoSoundEffectEnabled())

            effc->enableEchoSoundEffect(echoWDMix->value(), echoFDBack->value(), echoLDelay->value(),
                                        echoRDelay->value(), echoPanDelay->value());
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableFlanger()
{
        if(not song->isFinished())
        {
            if(effc->isFlangerSoundEffectEnabled())
            {
                effc->enableFlangerSoundEffect(flangerWDMix->value(), flangerDepth->value(), flangerFDBack->value(),
                                               flangerFreq->value()/100.0f, flangerTWForm->value(), flangerDelay->value()/10,
                                               flangerPhase->value());
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableGargle()
{
        if(not song->isFinished())
        {
            if(effc->isGargleSoundEffectEnabled())
            {
                effc->enableGargleSoundEffect(gargleRateHZ->value(), gargleSWForm->value());
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableCompressor()
{
        if(not song->isFinished())
        {
            if(effc->isCompressorSoundEffectEnabled())
            {
                effc->enableCompressorSoundEffect(compGain->value(), compAttack->value()/100.0f, compRelease->value(),
                                                  compThreshold->value(), compRatio->value(), compPreDelay->value()/10.0f);
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableDistort()
{
        if(not song->isFinished())
        {
            if(effc->isDistortionSoundEffectEnabled())
            {
                effc->enableDistortionSoundEffect(distGain->value(), distEdge->value(), distPEQCFreq->value(),
                                                  distPEQBandw->value(), distPreLPCutOff->value());
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::enableChorus()
{
        if(not song->isFinished())
        {
            if(effc->isChorusSoundEffectEnabled())
            {
                effc->enableChorusSoundEffect(chorusWDMix->value(),chorusDepth->value(), chorusFDBack->value(),
                                              chorusFreq->value()/10.0f, chorusSWForm->value(), chorusDelay->value()/10.0f,
                                              chorusPhase->value());
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setWReverbOnOff()
{
        if(not song->isFinished())
        {
            if(!effc->isWavesReverbSoundEffectEnabled())
            {
                effc->enableWavesReverbSoundEffect(wreverbGain->value(), wreverbMix->value()/10,
                                               wreverbTime->value()/1000, wreverbHFTimeRatio->value()/1000);
                wreverbOnOff->setText("ON");
            } else {
                effc->disableWavesReverbSoundEffect();
                wreverbOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setEchoOnOff()
{
        if(not song->isFinished())
        {
            if(!effc->isEchoSoundEffectEnabled())
            {
                effc->enableEchoSoundEffect(echoWDMix->value(), echoFDBack->value(), echoLDelay->value(),
                                            echoRDelay->value(), echoPanDelay->value());
                echoOnOff->setText("ON");
            } else {
                effc->disableEchoSoundEffect();
                echoOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setFlangerOnOff()
{
        if(not song->isFinished())
        {
            if(not effc->isFlangerSoundEffectEnabled())
            {
                effc->enableFlangerSoundEffect(flangerWDMix->value(), flangerDepth->value(), flangerFDBack->value(),
                                               flangerFreq->value()/100, flangerTWForm->value(), flangerDelay->value()/10,
                                               flangerPhase->value());
                flangerOnOff->setText("ON");
            } else {
                effc->disableFlangerSoundEffect();
                flangerOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setGargleOnOff()
{
        if(not song->isFinished())
        {
            if(not effc->isGargleSoundEffectEnabled())
            {
                effc->enableGargleSoundEffect(gargleRateHZ->value(), gargleSWForm->value());
                gargleOnOff->setText("ON");
            } else {
                effc->disableGargleSoundEffect();
                gargleOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setCompOnOff()
{
        if(not song->isFinished())
        {
            if(not effc->isCompressorSoundEffectEnabled())
            {
                effc->enableCompressorSoundEffect(compGain->value(), compAttack->value()/100.0f, compRelease->value(),
                                                  compThreshold->value(), compRatio->value(), compPreDelay->value()/10.0f);
                compOnOff->setText("ON");
            } else {
                effc->disableCompressorSoundEffect();
                compOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setDistortOnOff()
{
        if(not song->isFinished())
        {
            if(not effc->isDistortionSoundEffectEnabled())
            {
                effc->enableDistortionSoundEffect(distGain->value(), distEdge->value(), distPEQCFreq->value(),
                                                  distPEQBandw->value(), distPreLPCutOff->value());
                distOnOff->setText("ON");
            } else {
                effc->disableDistortionSoundEffect();
                distOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::setChorusOnOff()
{
        if(not song->isFinished())
        {
            if(not effc->isChorusSoundEffectEnabled())
            {
                effc->enableChorusSoundEffect(chorusWDMix->value(),chorusDepth->value(), chorusFDBack->value(),
                                              chorusFreq->value()/10.0f, chorusSWForm->value(), chorusDelay->value()/10.0f,
                                              chorusPhase->value());
                chorusOnOff->setText("ON");
            } else {
                effc->disableChorusSoundEffect();
                chorusOnOff->setText("OFF");
            }
        }
}

//-------------------------------------------------------------------------------
void HelllMain::loadFiles()
{
    QStringList fdirs =  QFileDialog::getOpenFileNames(this,"Open Sound Files", "C:/",
                         "MPEG-1 Audio Layer (*.mp2 *.mp3);; Free Lossless Audio Codec (*.flac);; Windows Media Audio (*.wma);;"
                           "Wave Audio File (*.wav);; Ogg Vorbis Audio (*.ogg);; Amiga Modules (*.mod);; Impulse Tracker Audio(*.it);;"
                           "Scream Tracker 3 Audio (*.s3d);; Fast Tracker 2 XM Audio (*.xm);;"
                           "All Supported Files (*.mp2 *.mp3 *.ogg *.flac *.xm *.it *.s3d *.wav *.wma *.mod");

    if(fdirs.size() == 0){return;}

    for(int i=0; i<fdirs.size(); i++)
    {
        if(fdirs[i] not_eq "")
            addTrack(fdirs.at(i));
    }

    playList->sortItems(AscendingOrder);
}

//-------------------------------------------------------------------------------
void HelllMain::initSong(QString fdir)
{
    //emptyBuffers();
    SE->removeAllSoundSources();

    if(not QFile::exists(fdir))
    {
        QString msg = "O HelllPlayer não conseguiu localizar o seguinte arquivo de áudio da lista de reprodução atual: \n\n";
                msg += fdir.toAscii().constData();
        msgbox("Arquivo de Áudio Não Encontrado", msg, MB_ICONERROR | MB_OK);

        return;
    }
    else if(not playButton->isEnabled())
    {
        enablePlayFunc(true);
    }

    file = SE->addSoundSourceFromFile(fdir.toAscii().constData(), ESM_AUTO_DETECT);
    song = SE->play2D(file, false, false, false, true);
    effc = song->getSoundEffectControl();

    timeBar->setMaximum(song->getPlayLength());
    playposSlider->setMaximum(song->getPlayLength());

    ID3_Tag *tag = new ID3_Tag(fdir.toAscii().data());

    trackInfo1->clear();
    trackInfo2->clear();

    if((ID3_GetTitle(tag) != NULL) and (ID3_GetArtist(tag) != NULL))
    {
        trackInfo1->insert(ID3_GetTitle(tag));
        trackInfo2->insert(tr("Artist: ") + ID3_GetArtist(tag) + tr("        Album: ") + ID3_GetAlbum(tag) );
        if(ID3_GetYear not_eq NULL){trackInfo2->insert(tr(" (") + ID3_GetYear(tag) + tr(")")); }
    }
    else
    {
        QFileInfo *info = new QFileInfo(fdir);
        trackInfo1->insert(info->fileName());
        free(info);
        info = NULL;
    }

    int plength = song->getPlayLength();
    clockMinMax->display((int)(plength / 60000));
    clockSecMax->display((int)(plength % 60000)/1000); plength=0;

    timer->start();

    normalize();

    setVolume();
    setPBSpeed();

    free(tag);
    tag  = NULL;
}

//-------------------------------------------------------------------------------
bool HelllMain::initSongFromArg(int argc, char* argv[])
{
    if(argv[1] != NULL)
    {
        QFileInfo info(argv[1]);
        QString suf = info.suffix();

        if((suf == "hpl") or (suf == "m3u") or (suf == "txt"))
        {
            QFile plist(info.fileName().toAscii().constData());
            plist.open(QIODevice::ReadOnly | QIODevice::Text);

            QTextStream in(&plist);

            while(not in.atEnd())
            {
            QString line = in.readLine();
            if(line != ""){ addTrack(line); }
            }

            playList->sortItems(AscendingOrder);

            nextTrack();
        }
        else
        {
            addTrack(argv[1]);
            initSong(argv[1]);
            return true;
        }
    }
    else
    {
        enablePlayFunc(false);
    }
}

//-------------------------------------------------------------------------------
void HelllMain::updateTime()
{
    unsigned int playpos = song->getPlayPosition();

    if(not playposSlider->isSliderDown())
    {
        playposSlider->setSliderPosition(playpos);
        timeBar->setValue(playposSlider->value());
    }

    clockSec->display((int)(playpos % 60000)/1000);
    clockMin->display((int)(playpos / 60000));

    if(song->isFinished())
    {
        clockSec->display(0);
        clockMin->display(0);

        if(not invertPlaySeq->isChecked())
        {nextTrack();}
        else
        {previousTrack();}

        if(startTrackPaused->isChecked()){ song->setIsPaused(true); }
    }
}

//-------------------------------------------------------------------------------
void HelllMain::getSysInfo()
{
    sigar_proc_cpu_get(sigar, getpid(), &proc);

    sysInfo1->clear();
    sysInfo1->insert(" CPU Usage: " + QString::number(proc.percent, 10.0).section("",0,4) + "%");
}

//-------------------------------------------------------------------------------
void HelllMain::addTrack(QString fdir)
{
    playList->insertItem(playList->count()+1, new QListWidgetItem(fdir));
}

//--------------------------------------------------------------------------------
void HelllMain::deleteSelected()
{
    for(int i=0; i<(playList->count()); i++)
    {
        if(playList->item(i)->isSelected())
        {
            playList->removeItemWidget(playList->takeItem(i));
        }
   }

    if(playList->count() < 1)
    {
        enablePlayFunc(false);
        trackInfo1->clear();
        trackInfo2->clear();
        clockMin->display(0);
        clockSec->display(0);
        clockMinMax->display(0);
        clockSecMax->display(0);
        setStop();
    }
}

//--------------------------------------------------------------------------------
bool HelllMain::nextTrack()
{
    if(randomPlaying->isChecked())
    {playList->setCurrentRow(qrand() % playList->count());}

    QListWidgetItem* trackNext = playList->item(playList->currentRow()+1);

    if(trackNext not_eq NULL)
    {
        playList->setCurrentRow(playList->currentRow()+1);

        initSong(trackNext->text());

        return true;
    }

    free(trackNext);
    trackNext = NULL;
}

//-------------------------------------------------------------------------------
bool HelllMain::previousTrack()
{
    if(randomPlaying->isChecked())
    {playList->setCurrentRow(qrand() % playList->count());}

    QListWidgetItem* trackPrev = playList->item(playList->currentRow()-1);

    if(trackPrev not_eq NULL)
    {
        playList->setCurrentRow(playList->currentRow()-1);

        initSong(trackPrev->text());

        return true;
    }

    free(trackPrev);
    trackPrev = NULL;
}

//------------------------------------------------------------------------------
void HelllMain::setTrack(QListWidgetItem *item)
{
    initSong(item->text());
}

//------------------------------------------------------------------------------
void HelllMain::loadPList()
{
    QString fdir = QFileDialog::getOpenFileName(this, "Open PlayList File", "C:/", "Helll Playlist Files (*.hpl);;"
                                                                                              "Windows Media Playlist Files (*.m3u);;"
                                                                                              "Text Files (*.txt);;");
    if(fdir == ""){ return; }

    QFile *plist = new QFile(fdir);
    plist->open(QIODevice::ReadOnly | QIODevice::Text);

    QTextStream *in = new QTextStream(plist);

    in->skipWhiteSpace();

    int fails=0, rows=0;
    QString line = "";

    trackInfo1->insert("Copiando Arquivos para a Playlist, Aguarde...");
    qApp->processEvents();

    while(not in->atEnd())
    {
        line = (in->readLine());
        if(line == ""){continue;}

        if(plist->exists(line))
        {
            addTrack(line);
            rows++;
        }
        else{  fails++;  }
    }

    if(fails > 0)
    {
        QString msg = ("");
        msg += "Existem "; msg += QString::number(fails);
        msg += " arquivos da lista de reprodução, em um total de "; msg += QString::number(rows+fails);
        msg += " arquivos, que não puderam ser carregados ou encontrados.\n";
        msgbox("Erro Ao Carregar Arquivos da Lista de Reprodução", msg, MB_ICONWARNING | MB_OK);
    }

    trackInfo1->clear(); trackInfo2->clear();
    trackInfo1->insert(QString::number(rows) + " arquivos copiados.  Carregando Playlist, Aguarde...");

    qApp->processEvents();

    playList->sortItems(AscendingOrder);
    initSong(playList->item(1)->text());

    plist->close();

    free(plist); plist = NULL;
    free(in);    in    = NULL;
}

//------------------------------------------------------------------------------
void HelllMain::savePList()
{
    QString fdir = QFileDialog::getSaveFileName(this, "Save PlayList As", "C:/", "Helll Playlist File (*.hpl);;"
                                                                                            "Text File (*.txt);;");

    if(fdir == ""){ return; }

    QFile *plist = new QFile(fdir);
    plist->open(QIODevice::WriteOnly | QIODevice::Text);

    for(int i=0; i<playList->count(); i++)
    {
        plist->write(playList->item(i)->text().toAscii().constData());
        plist->putChar('\n');
    }

    plist->close();

    free(plist); plist = NULL;
}

//------------------------------------------------------------------------------
void HelllMain::checkKeyboard()
{
    if(keyhit(0x23) and keyhit(0x50)){ setPlayPause(); } else   //END + P
    if(keyhit(0x23) and keyhit(0x4E)){ nextTrack(); } else      //END + N
    if(keyhit(0x23) and keyhit(0x42)){ previousTrack(); } else  //END + B
    if(keyhit(0x23) and keyhit(0x53)){ setStop(); }  else       //END + S
    if(keyhit(0x23) and keyhit(46))  { deleteSelected(); } else //END + Delete
    if(keyhit(0x23) and keyhit(0x52)){ randomPlaying->setChecked(!randomPlaying->isChecked()); } //END + R
}

//------------------------------------------------------------------------------
void HelllMain::enablePlayFunc(bool key)
{
    playButton->setEnabled(key);
    stopButton->setEnabled(key);
    nextButton->setEnabled(key);
    previousButton->setEnabled(key);
    deleteTracks->setEnabled(key);
    savePlayList->setEnabled(key);
    disableAllButton->setEnabled(key);

    playposSlider->setEnabled(key);
    volumeSlider->setEnabled(key);
    pbspeedSlider->setEnabled(key);

    wreverbOnOff->setEnabled(key);
    wreverbGain->setEnabled(key);
    wreverbHFTimeRatio->setEnabled(key);
    wreverbMix->setEnabled(key);
    wreverbTime->setEnabled(key);

    echoFDBack->setEnabled(key);
    echoLDelay->setEnabled(key);
    echoOnOff->setEnabled(key);
    echoPanDelay->setEnabled(key);
    echoRDelay->setEnabled(key);
    echoWDMix->setEnabled(key);

    flangerDelay->setEnabled(key);
    flangerDepth->setEnabled(key);
    flangerFDBack->setEnabled(key);
    flangerFreq->setEnabled(key);
    flangerOnOff->setEnabled(key);
    flangerPhase->setEnabled(key);
    flangerTWForm->setEnabled(key);
    flangerWDMix->setEnabled(key);

    gargleOnOff->setEnabled(key);
    gargleRateHZ->setEnabled(key);
    gargleSWForm->setEnabled(key);

    distEdge->setEnabled(key);
    distGain->setEnabled(key);
    distOnOff->setEnabled(key);
    distPEQBandw->setEnabled(key);
    distPEQCFreq->setEnabled(key);
    distPreLPCutOff->setEnabled(key);

    chorusDelay->setEnabled(key);
    chorusDepth->setEnabled(key);
    chorusFDBack->setEnabled(key);
    chorusFreq->setEnabled(key);
    chorusOnOff->setEnabled(key);
    chorusPhase->setEnabled(key);
    chorusSWForm->setEnabled(key);
    chorusWDMix->setEnabled(key);

    compAttack->setEnabled(key);
    compGain->setEnabled(key);
    compOnOff->setEnabled(key);
    compPreDelay->setEnabled(key);
    compRatio->setEnabled(key);
    compRelease->setEnabled(key);
    compThreshold->setEnabled(key);
}




}// End of HELLL






