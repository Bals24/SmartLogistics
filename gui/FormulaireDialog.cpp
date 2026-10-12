#include "FormulaireDialog.hpp"

#include <QComboBox>
#include <QDate>
#include <QDateEdit>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <QPushButton>
#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QVBoxLayout>

namespace
{
    // "125 000 Ar" -> 125000.0 ; "2.5 kg" -> 2.5
    double nombreDe(QString texte)
    {
        texte.remove(QRegularExpression("[^0-9,.]"));
        texte.replace(',', '.');
        return texte.toDouble();
    }

    QString groupeParTrois(long long valeur)
    {
        const QString chiffres = QString::number(valeur < 0 ? -valeur : valeur);
        QString resultat;
        int compteur = 0;
        for (int i = chiffres.size() - 1; i >= 0; --i)
        {
            if (compteur > 0 && compteur % 3 == 0)
            {
                resultat.prepend(' ');
            }
            resultat.prepend(chiffres[i]);
            ++compteur;
        }
        return valeur < 0 ? "-" + resultat : resultat;
    }

    QString formaterNombre(double valeur, const QString& suffixe)
    {
        if (suffixe.trimmed() == "kg")
        {
            return QString::number(valeur, 'f', 1) + suffixe;
        }
        return groupeParTrois(static_cast<long long>(valeur + 0.5)) + suffixe;
    }
}

FormulaireDialog::FormulaireDialog(const QString& titre,
                                   const QList<ChampFormulaire>& champs,
                                   const QStringList& valeursInitiales,
                                   QWidget* parent)
    : QDialog(parent),
      champs_(champs)
{
    setWindowTitle(titre);
    setModal(true);
    setMinimumWidth(460);

    QVBoxLayout* principal = new QVBoxLayout(this);
    principal->setContentsMargins(24, 24, 24, 20);
    principal->setSpacing(16);

    QLabel* entete = new QLabel(titre);
    entete->setObjectName("sectionTitle");
    principal->addWidget(entete);

    QFormLayout* formulaire = new QFormLayout();
    formulaire->setHorizontalSpacing(18);
    formulaire->setVerticalSpacing(12);

    for (int i = 0; i < champs_.size(); ++i)
    {
        const ChampFormulaire& champ = champs_[i];
        const QString initiale = i < valeursInitiales.size() ? valeursInitiales[i] : QString();

        QWidget* widget = nullptr;

        switch (champ.type)
        {
            case ChampFormulaire::Type::Choix:
            case ChampFormulaire::Type::ChoixEditable:
            {
                QComboBox* liste = new QComboBox();
                liste->addItems(champ.choix);

                // Une valeur deja presente (venant de la base) doit rester selectionnable
                if (!initiale.isEmpty() && liste->findText(initiale) < 0)
                {
                    liste->addItem(initiale);
                }
                if (!initiale.isEmpty())
                {
                    liste->setCurrentText(initiale);
                }
                liste->setEditable(champ.type == ChampFormulaire::Type::ChoixEditable);
                widget = liste;
                break;
            }

            case ChampFormulaire::Type::Date:
            {
                QDateEdit* date = new QDateEdit();
                date->setCalendarPopup(true);
                date->setDisplayFormat("dd/MM/yyyy");

                const QDate valeur = QDate::fromString(initiale, "dd/MM/yyyy");
                date->setDate(valeur.isValid() ? valeur : QDate::currentDate());
                widget = date;
                break;
            }

            case ChampFormulaire::Type::Nombre:
            {
                QLineEdit* saisie = new QLineEdit();
                saisie->setValidator(new QRegularExpressionValidator(
                    QRegularExpression("[0-9 .,]*"), saisie));

                const QString nombre = initiale.isEmpty()
                    ? QString("0")
                    : QString::number(nombreDe(initiale), 'f', champ.suffixe.trimmed() == "kg" ? 1 : 0);
                saisie->setText(nombre);

                if (!champ.suffixe.isEmpty())
                {
                    saisie->setPlaceholderText(champ.suffixe.trimmed());
                }
                widget = saisie;
                break;
            }

            case ChampFormulaire::Type::LectureSeule:
            {
                QLineEdit* saisie = new QLineEdit(initiale);
                saisie->setReadOnly(true);
                saisie->setFocusPolicy(Qt::NoFocus);
                widget = saisie;
                break;
            }

            case ChampFormulaire::Type::Texte:
            default:
            {
                widget = new QLineEdit(initiale);
                break;
            }
        }

        widgets_.append(widget);
        formulaire->addRow(champ.etiquette + (champ.requis ? " *" : ""), widget);
    }

    principal->addLayout(formulaire);

    QDialogButtonBox* boutons = new QDialogButtonBox();
    QPushButton* enregistrer = boutons->addButton("Enregistrer", QDialogButtonBox::AcceptRole);
    QPushButton* annuler = boutons->addButton("Annuler", QDialogButtonBox::RejectRole);
    enregistrer->setDefault(true);

    connect(enregistrer, &QPushButton::clicked, this, &QDialog::accept);
    connect(annuler, &QPushButton::clicked, this, &QDialog::reject);

    principal->addWidget(boutons);
}

QString FormulaireDialog::valeurDuChamp(int index) const
{
    const ChampFormulaire& champ = champs_[index];
    QWidget* widget = widgets_[index];

    switch (champ.type)
    {
        case ChampFormulaire::Type::Choix:
        case ChampFormulaire::Type::ChoixEditable:
            return static_cast<QComboBox*>(widget)->currentText().trimmed();

        case ChampFormulaire::Type::Date:
            return static_cast<QDateEdit*>(widget)->date().toString("dd/MM/yyyy");

        case ChampFormulaire::Type::Nombre:
        {
            const QString saisie = static_cast<QLineEdit*>(widget)->text().trimmed();
            return formaterNombre(nombreDe(saisie), champ.suffixe);
        }

        case ChampFormulaire::Type::LectureSeule:
        case ChampFormulaire::Type::Texte:
        default:
            return static_cast<QLineEdit*>(widget)->text().trimmed();
    }
}

QStringList FormulaireDialog::valeurs() const
{
    QStringList resultat;
    for (int i = 0; i < champs_.size(); ++i)
    {
        resultat << valeurDuChamp(i);
    }
    return resultat;
}

void FormulaireDialog::accept()
{
    for (int i = 0; i < champs_.size(); ++i)
    {
        if (champs_[i].requis && valeurDuChamp(i).isEmpty())
        {
            QMessageBox::warning(
                this,
                "Champ obligatoire",
                QString("Le champ « %1 » est obligatoire.").arg(champs_[i].etiquette));
            widgets_[i]->setFocus();
            return;
        }
    }

    QDialog::accept();
}