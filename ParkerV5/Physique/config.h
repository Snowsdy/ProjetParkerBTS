#ifndef CONFIG_H
#define CONFIG_H

#include <QList>
#include <QString>

class Config
{
public :
    // Vannes proportionnelles
    int PILOTAGE_GAUCHE;
    int PILOTAGE_DROIT;
    int PILOTAGE_PRESSION;

    // Capteurs pressions
    int SORTIE_DROITE;
    int CMD_PRESSION;
    int SORTIE_GAUCHE;
    int CMD_DROITE;
    int CMD_GAUCHE;

    // Nombre de cartes Relais et n° carte
    int NB_MODULE_RELAIS;
    int MODULE4;
    int MODULE5;
    int MODULE6;

    // Entrée TOR
    int AU= 0;
    int ETOR= 2;

    // Nom des devices NI
    QString devCapteurP;
    QString devVannesP;
    QString devRelaisMod4; // Pilotage Droit
    QString devRelaisMod5; // Pilotage Pression
    QString devRelaisMod6; // Pilotage Gauche
    QString devETOR; // Entrées TOR (AU et contact)

    // Activation du GPIB ou non
    bool ELEC;

private:
    // Liste commande GPIB
    QList<QString> listCmdGPIB;
    QList<QString> listAdrGPIB;
    int choix;  // Parker (0) ou Lycée (1)

public:
    Config(int choix);
    QList<QString> getListCmdGPIB();
    QList<QString> getListAdrGPIB();
    int getChoix();

    /// SETTERS :
    /// Vannes Proportionnelles :
    void setPILOTAGE_GAUCHE(const int &val);
    void setPILOTAGE_DROIT(const int &val);
    void setPILOTAGE_PRESSION(const int &val);
    /// Capteurs :
    void setSORTIE_DROITE(const int &val);
    void setCMD_PRESSION(const int &val);
    void setSORTIE_GAUCHE(const int &val);
    void setCMD_DROITE(const int &val);
    void setCMD_GAUCHE(const int &val);
    /// Relais :
    void setNB_MODULE_RELAIS(const int &val);
    /// Arrêt d'Urgence :
    void setAU(const int &val);
    /// Séquences de Tests :
    void setETOR(const int &val);
    /// Devices NI :
    void setDevCapteurP(const QString &val);
    void setDevVannesP(const QString &val);
    void setDevRelaisMod4(const QString &val);
    void setDevRelaisMod5(const QString &val);
    void setDevRelaisMod6(const QString &val);
    void setdevETOR(const QString &val);
    /// Activation GPIB :
    void setELEC(const bool &val);
};

#endif // CONFIG_H
