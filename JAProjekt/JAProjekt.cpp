#include "JAProjekt.h"
#include  "Bitmap.h"
#include "math.h"

JAProjekt::JAProjekt(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	
    connect(ui.fileInButton, &QPushButton::clicked, this, &JAProjekt::onFileInButtonClicked);
    connect(ui.fileOutButton, &QPushButton::clicked, this, &JAProjekt::onFileOutButtonClicked);
    connect(ui.makeButton, &QPushButton::clicked, this, &JAProjekt::onMakeButtonClicked);

}

void JAProjekt::isReadyToScale()
{
	if (ifFileInChoosen && ifFileOutChoosen && (ui.radioButtonAsm->isChecked() || ui.radioButtonCpp->isChecked()))
		ui.makeButton->setEnabled(true);
}

void JAProjekt::updatePaths()
{
	std::string pathIn = fileInPath.toLocal8Bit();
	fileInPathString = pathIn;

	fileOutPath = ui.fileOutlineEdit->text();
	std::string pathOut = fileOutPath.toLocal8Bit();
	fileOutPathString = pathOut;
}

void JAProjekt::getScaleAndThreads()
{
	QString scaleXQString = ui.lineEditX->text();
	scaleX = scaleXQString.toInt();

	QString scaleYQString = ui.lineEditY->text();
	scaleY = scaleYQString.toInt();

	QString threadsQString = ui.threadsSpinBox->text();
	threads = threadsQString.toInt();
}

void JAProjekt::onFileInButtonClicked()
{
    fileInPath = QFileDialog::getOpenFileName(this, "Otwieranie pliku", QDir::currentPath(), filter);
	ui.fileInlineEdit->setText(fileInPath);

	if(QString::compare(fileInPath,QString()) != 0)
	{
		QImage image;
		bool valid = image.load((fileInPath));

		if (valid)
			ui.imageLabel->setPixmap(QPixmap::fromImage(image));
	}

	updatePaths();

	if (lastN(fileInPathString, 4) == ".bmp")
		ifFileInChoosen = true;

	isReadyToScale();
}

void JAProjekt::onFileOutButtonClicked()
{
	fileOutPath = QFileDialog::getOpenFileName(this, "Wybieranie pliku wyjsciowego", QDir::currentPath(), filter);
	ui.fileOutlineEdit->setText(fileOutPath);

	updatePaths();
	
	if(lastN(fileOutPathString,4) == ".bmp")
		ifFileOutChoosen = true;
	else
		QMessageBox::warning(this, "Warning", "Nalezy wybrac plik z rozszerzeniem .bmp !");

	isReadyToScale();
}

void JAProjekt::onMakeButtonClicked()
{
	updatePaths();
	
	if (lastN(fileOutPathString, 4) == ".bmp")
	{
		getScaleAndThreads();
		
		Bitmap bmp(fileInPathString);


		if (bmp.bmpInfoHeader.bit_count == 24)
		{
			bmp.asmOrCpp = ui.radioButtonCpp->isChecked();

			if(bmp.write(fileOutPathString, scaleX, scaleY, bmp.asmOrCpp, threads))
			{
				double timeTaken = 0.0;

				if (bmp.asmOrCpp == true)
					timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(bmp.endCpp - bmp.beginCpp).count();
				else
					timeTaken = std::chrono::duration_cast<std::chrono::microseconds>(bmp.endAsm - bmp.beginAsm).count();

				timeTaken = timeTaken / 1000000.0;
				time = QString::number(timeTaken);
			}
			else
				QMessageBox::warning(this, "Warning", "Za duzy plik wynikowy! Prosze zmniejszyc skale");
		}
		else
			QMessageBox::warning(this, "Warning", "Nalezy wybrac bitmape 24 bitowa");
	}
	else
		QMessageBox::warning(this, "Warning", "Nalezy wybrac plik z rozszerzeniem .bmp !");
	
	ui.lineEditTime->setText(time + " sekundy");
}

std::string lastN(std::string input, int n)
{
	int inputSize = input.size();
	return (n > 0 && inputSize > n) ? input.substr(inputSize - n) : "";
}
