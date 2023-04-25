#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileSystemModel>
#include <QFileDialog>
#include <QMessageBox>
#include <stdint.h>
#include <QRadioButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDialogButtonBox>
#include <QGroupBox>
#include <QDebug>
#include <QMovie>
#include <QIcon>
#include "mosaiquemaker.cpp"
#include <QFont>
#include <QImageWriter>



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->centralWidget->setStyleSheet("background-color: #424340;");

    // Create a layout for the central widget
    QVBoxLayout *centralLayout = new QVBoxLayout(ui->centralWidget);
    ui->centralWidget->setLayout(centralLayout);
    centralLayout->setAlignment(Qt::AlignHCenter);

    // Create a horizontal layout for the buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    centralLayout->addLayout(buttonLayout);

    buttonLayout->setAlignment(Qt::AlignHCenter);

    // Add a spacer item to center the buttons

    // Add the second button to the button layout
    ui->pushButton_2->setFixedSize(200, 60);
    buttonLayout->addWidget(ui->pushButton_2);
    ui->pushButton_2->setStyleSheet("background-color: #137C8B; color: #FFFFFF;");

    // Add the first button to the button layout
    ui->pushButton->setFixedSize(200, 60);
    buttonLayout->addWidget(ui->pushButton);
    ui->pushButton->setStyleSheet("background-color: #137C8B; color: #FFFFFF;");


    // Add the result label to the central layout
    centralLayout->addWidget(ui->result, 1);


    // Set the size policy for the result and nbfilescharges labels to expand horizontally and vertically
    ui->result->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->result->setStyleSheet("background-color: #DEDEDE; color: #000000;");
    ui->result->setText("Aucune Image créée pour l'instant");
    ui->result->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);


    centralLayout->addWidget(ui->pushButton_3);

    QHBoxLayout *convertir = new QHBoxLayout();
    centralLayout->addLayout(convertir);
    convertir->addWidget(ui->pushButton_3);
    ui->pushButton_3->setFixedSize(200, 40);
    ui->pushButton_3->setStyleSheet("background-color: #137C8B; color: #FFFFFF;");


}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_2_clicked()
{
    QDialog dialog1;
    dialog1.setWindowTitle("Sélectionner une image");
    dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

    this->PATH = QFileDialog::getExistingDirectory(&dialog1, tr("Ouvrir un dossier"), "/home", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    // Traitement du répertoire sélectionné...
    QFileSystemModel *Dossier = new QFileSystemModel;
    Dossier->setRootPath(this->PATH);
    QDir *choosedfolder=new QDir(this->PATH);
    uint nbFiles=choosedfolder->count();
    QString text=QString::number(nbFiles)+" fichiers chargés";
    QFont normalFont("Montserrat", 10, QFont::Normal);
    QString normalPart = "Ouvrir une base de données\n";
    QString labelText = normalPart+ text ;
    ui->pushButton_2->setText(labelText);
    ui->pushButton_2->setFont(normalFont);
    delete Dossier;
}

void MainWindow::on_pushButton_clicked()
{
    mosaiquemaker moe;
    QMovie* movie = new QMovie("../loading.gif");

    if(PATH==""){
        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle("Erreur");
        msgBox->setText("Un dossier contenant la base de données doit être ouvert !");
        msgBox->setIcon(QMessageBox::Critical);

        msgBox->setStyleSheet("background-color: #424340;color:#FFFFFF;");


        msgBox->exec();

    }else{

        QDialog dialog;
        dialog.setWindowTitle("Choix type");
        dialog.setStyleSheet("background-color: #424340;color:#FFFFFF;");

        // Création du QGroupBox
        QGroupBox *groupBox = new QGroupBox("Quel type de Mosaĩques voulez-vous choisir ?");
        QGroupBox *groupBoxIcon = new QGroupBox("Examples");

        // Création des QRadioButton
        QRadioButton *radioButton1 = new QRadioButton("Par régions");
        QRadioButton *radioButton2 = new QRadioButton("Par contours");
        QRadioButton *radioButton3 = new QRadioButton("Animée");

        QIcon *icon2=new QIcon("/home/fabien/Documents/FDS/M1/projet_image/Projet_Image/picture.png");
        QIcon *icon1=new QIcon("/home/fabien/Documents/FDS/M1/projet_image/Projet_Image/picture.png");
        QIcon *icon3=new QIcon("/home/fabien/Documents/FDS/M1/projet_image/Projet_Image/picture.png");


        // Définition du bouton par défaut
        radioButton1->setChecked(true);

        // Ajout des QRadioButton au QGroupBox
        QHBoxLayout *groupBoxLayout = new QHBoxLayout;
        groupBoxLayout->addWidget(radioButton1);
        groupBoxLayout->addWidget(radioButton2);
        groupBoxLayout->addWidget(radioButton3);
        groupBox->setLayout(groupBoxLayout);


        QHBoxLayout *groupIconLayout = new QHBoxLayout;
        QLabel *iconLabel1 = new QLabel();
        iconLabel1->setPixmap(QIcon("../exempleclassique.png").pixmap(120,120));
        groupIconLayout->addWidget(iconLabel1);

        QLabel *iconLabel2 = new QLabel();
        iconLabel2->setPixmap(QIcon("../exemplevisage.png").pixmap(200,200));
        groupIconLayout->addWidget(iconLabel2);

        QLabel *iconLabel3 = new QLabel();
        iconLabel3->setMovie(new QMovie("../exemplegif.gif"));
        iconLabel3->movie()->setScaledSize(QSize(120, 120));
        iconLabel3->movie()->start();
        groupIconLayout->addWidget(iconLabel3);
        groupBoxIcon->setLayout(groupIconLayout);



        // Création du QDialogButtonBox
        QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

        // Connexion des boutons du QDialogButtonBox
        QObject::connect(buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
        QObject::connect(buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

        QVBoxLayout *GroupChoixEtExamples= new QVBoxLayout;
        GroupChoixEtExamples->addWidget(groupBox);
        GroupChoixEtExamples->addWidget(groupBoxIcon);

        QWidget *widgetGroupChoixEtExamples = new QWidget;
        widgetGroupChoixEtExamples->setLayout(GroupChoixEtExamples);

        // Ajout du QGroupBox et du QDialogButtonBox au QVBoxLayout
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(widgetGroupChoixEtExamples);
        layout->addWidget(buttonBox);

        // Configuration du QDialog avec le QVBoxLayout
        dialog.setLayout(layout);

        dialog.setGeometry(100, 100, 350, 250); // Définit la position et la taille du QDialog

        // Affichage du QDialog
        if (dialog.exec() == QDialog::Accepted) {
            if (radioButton1->isChecked()) {
                QDialog dialog1;
                dialog1.setWindowTitle("Sélectionner une image");
                dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

                this->filename= QFileDialog::getOpenFileName(&dialog1, tr("Choisissez une Image"), "/home", tr("Image Files (*.ppm)"));
                if(this->filename!=""){
                    if (movie->isValid()) {
                        QSize labelSize = ui->result->size();
                        movie->setScaledSize(labelSize);
                        ui->result->setMovie(movie);
                        movie->start();
                        ui->result->update();
                    } else {
                        qDebug() << "Failed to load GIF";
                    }
                    qDebug() << "Choix 1 sélectionné";
                    this->setCursor(Qt::WaitCursor);

                    std::string out=(this->filename.split(".")[0]+"Sortie."+this->filename.split(".")[1]).toStdString();
                    char *cstr1 = new char[this->filename.length() + 1];
                    strcpy(cstr1, out.c_str());
                    char *cstr = new char[this->filename.length() + 1];
                    strcpy(cstr, (this->filename.toStdString()).c_str());

                    int res=moe.makeMosaique(1,cstr,cstr1,this);

                    if(res==-1){
                        ui->result->setPixmap(QPixmap());
                    }else if(res==1){


                        ui->result->setPixmap(QPixmap(cstr1)); // Récupérer la taille du QLabel
                        QSize labelSize = ui->result->size();

                        // Redimensionner l'image à la taille du QLabel
                        QPixmap scaledImage = ui->result->pixmap()->scaled(labelSize, Qt::KeepAspectRatio);

                        // Appliquer l'image redimensionnée au QLabel
                        ui->result->setPixmap(scaledImage);


                    }

                    // faire quelque chose pour le choix 1
                }
            } else if (radioButton2->isChecked()) {

                qDebug() << "Choix 2 sélectionné";
                QDialog dialog1;
                dialog1.setWindowTitle("Sélectionner une image");
                dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

                if (movie->isValid()) {
                    QSize labelSize = ui->result->size();
                    movie->setScaledSize(labelSize);
                    ui->result->setMovie(movie);
                    movie->start();
                    ui->result->update();
                } else {
                    qDebug() << "Failed to load GIF";
                }
                this->filename= QFileDialog::getOpenFileName(&dialog1, tr("Choisissez une Image"), "/home", tr("Image Files (*.ppm)"));
                if(this->filename!=""){

                    this->setCursor(Qt::WaitCursor);

                    //                    std::string out=(this->filename.split(".")[0]+"Sortie."+this->filename.split(".")[1]).toStdString();
                    //                    char *cstr1 = new char[this->filename.length() + 1];
                    //                    strcpy(cstr1, out.c_str());
                    char *cstr = new char[this->filename.length() + 1];
                    strcpy(cstr, (this->filename.toStdString()).c_str());
                    int res=moe.makeMosaique(2,cstr,"../cstr1.ppm",this);
                    if(res==-1){
                        ui->result->setPixmap(QPixmap());
                    }else if(res==1){
                        ui->result->setPixmap(QPixmap("../cstr1.ppm")); // Récupérer la taille du QLabel
                        QSize labelSize = ui->result->size();

                        // Redimensionner l'image à la taille du QLabel
                        QPixmap scaledImage = ui->result->pixmap()->scaled(labelSize, Qt::KeepAspectRatio);

                        // Appliquer l'image redimensionnée au QLabel
                        ui->result->setPixmap(scaledImage);
                    }

                }
            }else if (radioButton3->isChecked()) {
                qDebug() << "Choix 3 sélectionné";
                // faire quelque chose pour le choix 3


                QDialog dialog1;
                dialog1.setWindowTitle("Sélectionner une image");
                dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

                QStringList s= QFileDialog::getOpenFileNames(&dialog1, tr("Choisissez les images de votre gif"), "/home", tr("Image Files (*.ppm)"));
                if(s.size()>0){
                    if (movie->isValid()) {
                        QSize labelSize = ui->result->size();
                        movie->setScaledSize(labelSize);
                        ui->result->setMovie(movie);
                        movie->start();
                        ui->result->update();
                    } else {
                        qDebug() << "Failed to load GIF";
                    }
                    QMessageBox *msgBox = new QMessageBox();
                    msgBox->setWindowTitle("À Savoir");
                    msgBox->setText("Sachez que pour la suite Image Magick est nécéssaire, si cette blibliothèque n'est pas installée l'application ne fonctionnera pas\nVous pouvez l'installer via:\n-https://imagemagick.org/script/download.php pour Windows ou Mac\n-sudo apt-get install imagemagick libmagick++-dev pour Ubuntu!");
                    msgBox->setIcon(QMessageBox::Information);

                    msgBox->setStyleSheet("background-color: #424340;color:#FFFFFF;");

                    msgBox->exec();
                    this->setCursor(Qt::WaitCursor);


                    //Demande la taille des mosaiques à l'utilisateur
                    QDialog dialog;
                    dialog.setStyleSheet("background-color: #424340;color:#FFFFFF;");

                    dialog.setWindowTitle("Taille Imagettes");
                    QVBoxLayout layout(&dialog) ;


                    QLabel text;
                    text.setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);

                    text.setText("Sélectionnez la taille des imagettes utilisées pour votre mosaïque : ");
                    layout.addWidget(&text);

                    QComboBox comboBox;
                    comboBox.setFixedWidth(100);

                    comboBox.addItem("2x2");
                    comboBox.addItem("4x4");
                    comboBox.addItem("8x8");
                    comboBox.addItem("16x16");
                    comboBox.addItem("32x32");
                    comboBox.addItem("64x64");
                    comboBox.addItem("128x128");
                    comboBox.addItem("256x256");

                    layout.addWidget(&comboBox);

                    QPushButton ok;
                    ok.setText("OK");

                    ok.setFixedWidth(50);
                    layout.addWidget(&ok);

                    layout.addWidget(&text, 0, Qt::AlignHCenter);
                    layout.addWidget(&comboBox, 0, Qt::AlignHCenter);
                    layout.addWidget(&ok, 0, Qt::AlignHCenter);
                    dialog.connect(&ok, &QPushButton::clicked, &dialog, &QDialog::accept);

                    // Affichage du QDialog
                    int res=dialog.exec();
                    QStringList arguments;
                    foreach (filename, s) {
                        std::string out=(filename.split(".")[0]+"Sortie."+filename.split(".")[1]).toStdString();
                        char *cstr1 = new char[filename.length() + 1];
                        strcpy(cstr1, out.c_str());
                        char *cstr = new char[filename.length() + 1];
                        strcpy(cstr, (filename.toStdString()).c_str());
                        int result=moe.makeMosaique(1,&filename.toStdString()[0],&(filename.split(".")[0]+"Sortie."+filename.split(".")[1]).toStdString()[0],this,res);
                       if(result==-1){
                           ui->result->setPixmap(QPixmap());
                        }else if(result==1){
                           arguments.append(cstr1);
                        }
                    }

                    moe.gif(arguments);

                    QMovie *movie = new QMovie("../migif.gif");
                    ui->result->setMovie(movie);
                    movie->start();

                }

            }
        }

        this->setCursor(Qt::ArrowCursor);



    }
}

void MainWindow::on_pushButton_3_clicked()
{
    if(ui->result->pixmap()!=nullptr){
        QPixmap pixmap=ui->result->pixmap()->copy();

        QStringList mimeTypeFilters;
        mimeTypeFilters << "image/png"<<"image/ppm";
        QString selectedFilter;

        QDialog dialog1;
        dialog1.setWindowTitle("Sélectionner une image");
        dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

        QString fileName = QFileDialog::getSaveFileName(&dialog1, "Enregistrer l'image", QDir::homePath(), mimeTypeFilters.join(";;"), &selectedFilter);

        if (!fileName.isEmpty()) {
            if (QFileInfo(fileName).suffix().isEmpty() && selectedFilter=="image/png") {
                fileName.append(".png");
            }else if(QFileInfo(fileName).suffix().isEmpty() && selectedFilter=="image/ppm"){
                fileName.append(".ppm");
            }

            if (pixmap.save(fileName)) {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle("Succès");
                msgBox->setText("L'image a été enregistrée avec succès.");
                msgBox->setIcon(QMessageBox::Information);

                msgBox->setStyleSheet("background-color: #424340;color:#FFFFFF;");
                msgBox->exec();

            } else {
                QMessageBox *msgBox = new QMessageBox();
                msgBox->setWindowTitle("Erreur");
                msgBox->setText("Impossible d'enregistrer l'image.");
                msgBox->setIcon(QMessageBox::Warning);

                msgBox->setStyleSheet("background-color: #424340;color:#FFFFFF;");
                msgBox->exec();

            }
        }
    }

    if(ui->result->movie()!=nullptr){

        QDialog dialog1;
        dialog1.setWindowTitle("Sélectionner une image");
        dialog1.setStyleSheet("background-color: #424340;color:#FFFFFF;");

        QString selectedFilter;
        QString saveFileName = QFileDialog::getSaveFileName(&dialog1, "Enregistrer gif", QDir::homePath(), "GIF files (*.gif)");
        saveFileName.append(".gif");

        if (saveFileName.isEmpty()) {
            return;
        }

        QProcess myProcess;

        // Connect the finished signal to a slot
        //connect(myProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(onProcessFinished(int, QProcess::ExitStatus)));

        // Start the process
        QStringList args={"../migif.gif",saveFileName};
        myProcess.start("../download", args);

        myProcess.waitForFinished(-1);

        QMessageBox *msgBox = new QMessageBox();
        msgBox->setWindowTitle("Succès");
        msgBox->setText("Le gif a été enregistré avec succès.");
        msgBox->setIcon(QMessageBox::Information);
        msgBox->setStyleSheet("background-color: #424340;color:#FFFFFF;");
        msgBox->exec();


    }
}
