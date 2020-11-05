#include "JAProjekt.h"
#include  "Bitmap.h"

JAProjekt::JAProjekt(QWidget* parent)
    : QWidget(parent)
{
    ui.setupUi(this);
	unsigned int n = std::thread::hardware_concurrency();
	std::string threads = "Sugerowana liczba w\304\205tk\303\263w: " + std::to_string(n);

	ui.howManyThreads->setText(QString::fromStdString(threads));
	ui.threadsSpinBox->setValue(n);
	
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
	QString scaleXQString = ui.scaleXSpinBox->text();
	scaleX = scaleXQString.toInt();

	QString scaleYQString = ui.scaleYSpinBox->text();
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
	fileOutPath = QFileDialog::getSaveFileName(this, "Wybieranie pliku wyjsciowego", QDir::currentPath(), filter);
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

		if(!bmp.tooBig)
		{
			if (bmp.bmpInfoHeader.bit_count == 24)
			{
				bmp.asmOrCpp = ui.radioButtonCpp->isChecked();

				if (bmp.write(fileOutPathString, scaleX, scaleY, bmp.asmOrCpp, threads))
				{
					time = QString::number(bmp.time);

					QMessageBox::information(this, "Success", Q("Udało się!"));
				}
				else
					QMessageBox::warning(this, "Warning", Q("Za duży plik wynikowy! Proszę zmniejszyć skalę"));
			}
			else
				QMessageBox::warning(this, "Warning", Q("Należy wybrać bitmapę 24 bitową"));
		}
		else
			QMessageBox::warning(this, "Warning", Q("Za duży plik wejściowy! Proszę wybrać inny plik"));
	}
	else
		QMessageBox::warning(this, "Warning", Q("Należy wybrać plik z rozszerzeniem .bmp !"));
	
	ui.lineEditTime->setText(time + " sekundy");	//setting time in GUI
}

std::string lastN(std::string input, int n)
{
	int inputSize = input.size();
	return (n > 0 && inputSize > n) ? input.substr(inputSize - n) : "";
}
