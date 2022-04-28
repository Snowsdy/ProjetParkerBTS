#include "carteni.h"


CarteNI::CarteNI()
{
    //
}

QString CarteNI::getDeviceList()
{
    device.clear();
    QString message, final;
    char *devices = nullptr, *product_type = nullptr;
    int32 stringLength = 0, status;
    int buffer = 0, devserialnum, is_simulated;

    stringLength = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, &devices[0]);

    if (stringLength <= 0) {
        return message + "Aucun périphérique de connu !";
    } else {
        devices = static_cast<char*>(malloc(stringLength));
        /* Device Name */
        status = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, devices, stringLength);

        if (status < 0) {
            return "Erreur dans la fct GetDevicesNames() dans CarteNI !";
        } else {
            QString devicesString(devices);
            device = devicesString.split(',');

            message.append("Numbers Of Devices : " + QString::number(device.length()) + "\n");
            message.append("---------------------------------------------------------------------------------\n");
            message.append("| Device Name | Product Type | # Serial Number | Is Simulated ? |\n");
            message.append("---------------------------------------------------------------------------------\n");

            final.append(message);
            for (QString dev : device) {
                if (dev.contains(' '))
                    dev.remove(' ');
                /* Product Type */
                buffer = DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_ProductType, nullptr);
                product_type = static_cast<char*>(malloc(buffer));
                DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_ProductType, product_type, buffer);
                /* Serial Number */
                DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_SerialNum, &devserialnum, 1);
                /* Is Simulated ? */
                DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_IsSimulated, &is_simulated, 1);
                /* Show Info */
                final.append("| " + dev + " | " + product_type + " | " + QString::number(devserialnum, 16).toUpper() + " | " + (is_simulated ? "Oui" : "Non") + " |");
                final.append('\n');
            }

            free(devices);
            free(product_type);
            return final;
        }
    }
}

QStringList CarteNI::getDeviceNames()
{
    device.clear();
    char *devices = nullptr;
    int32 stringLength = 0, status;
    stringLength = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, &devices[0]);
    if (stringLength <= 0) {
        return QStringList();
    } else {
        devices = static_cast<char*>(malloc(stringLength));
        status = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, devices, stringLength);
        if (status < 0) {
            return QStringList();
        } else {
            QString devicesString(devices);
            device = devicesString.split(',');
            for (int i = 0; i < device.length(); i++) {
                if (device[i].contains(' '))
                    device[i].remove(' ');
            }
            return device;
        }
    }
}

QStringList CarteNI::getProductType()
{
    device.clear();
    QStringList products;
    char *devices = nullptr, *product_type = nullptr;
    int32 stringLength = 0, status;
    stringLength = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, &devices[0]);
    if (stringLength <= 0) {
        return QStringList();
    } else {
        devices = static_cast<char*>(malloc(stringLength));
        status = DAQmxGetSystemInfoAttribute(DAQmx_Sys_DevNames, devices, stringLength);
        if (status < 0) {
            return QStringList();
        } else {
            QString devicesString(devices);
            device = devicesString.split(',');
            for (QString dev : device) {
                if (dev.contains(' '))
                    dev.remove(' ');
                status = DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_ProductType, nullptr);
                product_type = static_cast<char*>(malloc(status));
                DAQmxGetDeviceAttribute(dev.toLatin1().data(), DAQmx_Dev_ProductType, product_type, status);
                products.append(dev + " " + product_type);
            }
            return products;
        }
    }
}

/**
 * @brief
 * int option :
 * 1 - Nombre d'entrées analogiques /
 * 2 - Nombre de sorties analogiques /
 * 3 - Nombre d'entrées numériques /
 * 4 - Nombre de sorties numériques /
 * 5 - Nombre de lignes par port numérique en entrée /
 * 6 - Nombre de lignes par port numérique en sortie
 */
QString CarteNI::getIO(QString dev, int option)
{
    char* channelnames = nullptr;
    int32 buffer, infotype;
    QString final;

    switch (option) {
    case 1:
        infotype = DAQmx_Dev_AI_PhysicalChans;
        break;
    case 2:
        infotype = DAQmx_Dev_AO_PhysicalChans;
        break;
    case 3:
        infotype = DAQmx_Dev_DI_Ports;
        break;
    case 4:
        infotype = DAQmx_Dev_DO_Ports;
        break;
    case 5:
        infotype = DAQmx_DI_NumLines;
        break;
    case 6:
        infotype = DAQmx_DO_NumLines;
        break;
    default:
        return nullptr;
    }

    if ((option >= 0) && (option <= 6)) {
        /// DO Number of Lines :
        if(option == 6) {
            buffer = DAQmxGetDevDOLines(dev.toLatin1().data(), nullptr, 0);
            if (buffer > 0) {
                channelnames = static_cast<char*>(malloc(buffer));
                DAQmxGetDevDOLines(dev.toLatin1().data(), channelnames, buffer);
                final.append(QString(channelnames).remove(' '));
                free(channelnames);
                return final;
            } else {
                return "0";
            }
            /// DI Number of Lines :
        } else if (option == 5) {
            buffer = DAQmxGetDevDILines(dev.toLatin1().data(), nullptr, 0);
            if (buffer > 0) {
                channelnames = static_cast<char*>(malloc(buffer));
                DAQmxGetDevDILines(dev.toLatin1().data(), channelnames, buffer);
                final.append(QString(channelnames).remove(' '));
                free(channelnames);
                return final;
            } else {
                return "0";
            }
        }
        buffer = DAQmxGetDeviceAttribute(dev.toLatin1().data(), infotype, nullptr);
        if (buffer > 0) {
            channelnames = static_cast<char*>(malloc(buffer));
            DAQmxGetDeviceAttribute(dev.toLatin1().data(), infotype, channelnames, buffer);
            final.append(QString(channelnames).remove(' '));
            free(channelnames);
            return final;
        } else {
            return "0";
        }
    } else {
        return "Erreur dans getIO() dans CarteNI !";
    }
}
