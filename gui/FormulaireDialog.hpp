#pragma once

#include <QDialog>
#include <QList>
#include <QString>
#include <QStringList>

class QWidget;

/*
 * Description d'un champ du formulaire d'ajout / modification.
 * Un champ correspond a une colonne du tableau.
 */
struct ChampFormulaire
{
    enum class Type
    {
        Texte,           // saisie libre
        Choix,           // liste fermee
        ChoixEditable,   // liste + saisie libre
        Date,            // calendrier (jj/MM/aaaa)
        Nombre,          // nombre + unite (km, Ar, kg, colis)
        LectureSeule     // affiche seulement (identifiant, valeurs calculees)
    };

    QString etiquette;
    Type type = Type::Texte;
    QStringList choix;        // pour Choix / ChoixEditable
    QString suffixe;          // pour Nombre : " km", " Ar", " kg", " colis"
    bool decimal = false;     // pour Nombre : une decimale (poids d'un colis, ex : 2.5 kg)
    QString defaut;           // valeur proposee a l'ajout (sinon 0 / premier choix)
    bool requis = false;
};

class FormulaireDialog : public QDialog
{
public:
    FormulaireDialog(const QString& titre,
                     const QList<ChampFormulaire>& champs,
                     const QStringList& valeursInitiales,
                     QWidget* parent = nullptr);

    // Valeurs saisies, dans le meme ordre que les champs, formatees comme
    // dans le tableau ("125 000 Ar", "22/09/2026", ...).
    QStringList valeurs() const;

    void accept() override;

private:
    QString valeurDuChamp(int index) const;

    QList<ChampFormulaire> champs_;
    QList<QWidget*> widgets_;
};