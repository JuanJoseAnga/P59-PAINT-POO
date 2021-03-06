#include "principal.h"
#include "ui_principal.h"

#define DEFAULT_ANCHO 3

Principal::Principal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Principal)
{
    ui->setupUi(this);
    // Instanciando la imagen (creando)
    mImagen = new QImage(this->size(),QImage::Format_ARGB32_Premultiplied);
    // Rellenar la imagen de color blanco
    mImagen->fill(Qt::white);
    // Instanciar el Painter a partir de la imagen
    mPainter = new QPainter(mImagen);
    mPainter->setRenderHint(QPainter::Antialiasing);
    // Inicializar otras variables
    mPuedeDibujar = false;
    mColor = Qt::black;
    mAncho = DEFAULT_ANCHO;
    mNumLineas = 0;
    Messi="";
}

Principal::~Principal()
{
    delete ui;
    delete mPainter;
    delete mImagen;
}

void Principal::rectangulos()
{
    mPainter->drawRect(mInicial.x(),mInicial.y(),mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
}

void Principal::circulo()
{
    mPainter->drawEllipse(mInicial.x(), mInicial.y(), mFinal.x()-mInicial.x(),mFinal.y()-mInicial.y());
}

void Principal::acciones()
{
    if(Messi=="Dios"){
        rectangulos();
        mNumLineas = mNumLineas+4;
        ui->statusbar->showMessage(tr("Número de líneas: ") + QString::number(mNumLineas));
    }
    else if(Messi=="The Best"&&!ui->actionLibre->isChecked()){
        mPainter->drawLine(mInicial, mFinal);
        ui->statusbar->showMessage(tr("Número de líneas: ") + QString::number(++mNumLineas));
    }
    else if(Messi=="BdOro"){
        circulo();
        ui->statusbar->showMessage(tr("Número de líneas: ") + QString::number(++mNumLineas));
    }
}

void Principal::paintEvent(QPaintEvent *event)
{
    // Crear el painter de la ventana principal
    QPainter painter(this);
    // Dibujar la imagen
    painter.drawImage(0, 0, *mImagen);
    // Acepatr el evento
    event->accept();
}

void Principal::mousePressEvent(QMouseEvent *event)
{
    mPuedeDibujar = true;
    mInicial = event->pos();
    event->accept();
}

void Principal::mouseMoveEvent(QMouseEvent *event)
{
    // Validar si se puede dibujar
    if ( !mPuedeDibujar ) {
        event->accept();
        return;
    }
    // Capturar el punto donde se suelta el mouse
    mFinal = event->pos();

    pincel.setColor(mColor);
    pincel.setWidth(mAncho);
    // Dibujar una linea
    mPainter->setPen(pincel);
    if(Messi== "The Best"&&ui->actionLibre->isChecked()){
        mPainter->drawLine(mInicial, mFinal);
        // actualizar el punto inicial
        mInicial = mFinal;
    }
    // Mostrar el número de líneas en la barra de estado
    ui->statusbar->showMessage(tr("Número de líneas:  ") + QString::number(++mNumLineas));
    // Actualizar la interfaz
    update();

}

void Principal::mouseReleaseEvent(QMouseEvent *event)
{
    acciones();
    mPuedeDibujar = false;
    // Aceptar el vento
    event->accept();
    //Actualizar la interfaz
    update();

}


void Principal::on_actionAncho_triggered()
{
    mAncho = QInputDialog::getInt(this,
                                  tr("Ancho del pincel"),
                                  tr("Ingrese el ancho del pincel de dibujo"),
                                  mAncho,
                                  1, 100);
}

void Principal::on_actionSalir_triggered()
{
    this->close();
}

void Principal::on_actionColor_triggered()
{
    mColor = QColorDialog::getColor(mColor,
                                    this,
                                    tr("Color del pincel"));
}

void Principal::on_actionNuevo_triggered()
{
    mImagen->fill(Qt::white);
    mNumLineas = 0;
    update();
}

void Principal::on_actionGuardar_triggered()
{
    QString nombreArchivo = QFileDialog::getSaveFileName(this,
                                                         tr("Guardar imagen"),
                                                         QString(),
                                                         "Imágenes (*.png)");
    if ( !nombreArchivo.isEmpty() ){
        if (mImagen->save(nombreArchivo))
            QMessageBox::information(this,
                                     tr("Guardar imagen"),
                                     tr("Archivo almacenado en: ") + nombreArchivo);
        else
            QMessageBox::warning(this,
                                 tr("Guardar imagen"),
                                 tr("No se pudo almacenaar la imagen."));
    }
}

void Principal::on_actionLineas_triggered()
{
    Messi="The Best";
}
void Principal::on_actionRect_nculos_triggered()
{
    Messi="Dios";
}
void Principal::on_actionCircunferencias_triggered()
{
    Messi="BdOro";
}


