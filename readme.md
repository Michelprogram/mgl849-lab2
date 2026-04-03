# MGL849 - Laboratoire 2 : Panneau de contrôle des gaz


Système multi-threads en C pour la **réception**, la **décision**, l’**affichage** (matrice LED) et la **journalisation de métriques** autour de données de capteurs gaz.

## Objectif du projet

Ce projet met en œuvre une architecture concurrente basée sur :

- des **threads POSIX** (`pthread`)
- des **sémaphores** (`sem_t`)
- une **file intrusive** partagée entre threads
- une logique de **décision** qui produit des commandes réseau
- un affichage d’état via **matrice LED**
- une sortie de **métriques** dans un fichier de log

Le programme reçoit des trames de capteurs de type `LGxNN` (ex: `LG112`, `LG245`), les parse, calcule un niveau d’alarme, puis déclenche des actions.

---

## Architecture globale

### Threads principaux

1. **`task_socket_receiver`**
   - Se connecte au serveur de réception.
   - Lit les messages bruts.
   - Parse les valeurs gaz (`parsed_gas`).
   - Enfile les données dans `SharedData.messages`.
   - Signale `sem_new_data`.

2. **`task_decision`**
   - Attend `sem_new_data`.
   - Défile un message parser.
   - Génère des commandes d’alerte/action (`decision_engine`).
   - Envoie les commandes via socket.
   - Met à jour :
     - `alarm_levels[]` + `sem_new_alarm`
     - `metrics_snap` + `sem_new_metrics`

3. **`task_display`**
   - Attend `sem_new_alarm`.
   - Lit `alarm_levels[]`.
   - Met à jour la matrice LED via `led_matrix_set_gas_level`.

4. **`task_metrics`**
   - Attend `sem_new_metrics`.
   - Copie atomiquement `metrics_snap`.
   - Calcule le temps écoulé.
   - Écrit une ligne dans `metrics.log`.

---

## Structure des données partagées

La structure `SharedData` (dans `include/shared_data.h`) contient :

- `messages` : file des mesures parsées (`gas_parsed_t`)
- `lock` : mutex de protection
- `sem_new_data` : signal de nouvelle donnée brute traitée
- `alarm_levels[3]` + `sem_new_alarm`
- snapshots métriques (`metrics_snap`) + `sem_new_metrics`
- tableaux `gas_values[]` et `gas_prev[]` (suivi des valeurs)

---

## Format attendu des messages capteurs

Le parseur (`src/parser.c`) attend des messages de la forme :

- Préfixe `LG`
- Index gaz `1..3`
- Valeur entière ensuite

Exemples valides :

- `LG10`
- `LG212`
- `LG349`

Niveau d’alarme calculé :

- `0..5`   → `NONE`
- `6..20`  → `LOW`
- `21..49` → `MEDIUM`
- `>=50`   → `HIGH`

---

# Utilisation

À la racine du projet, il y a un makefile avec la liste des commandes possibles.

## Compilation

```bash
make
```

## Nettoyage
```bash
make clean
```

### Recompilation complète
```bash
make rebuild
```

## Utilisation

### Exécution avec serveur local
```bash
sudo make run
```

### Exécution avec serveur distant
```bash
sudo make run IP=192.168.1.100 PORT_RECEIVER=1231 PORT_SENDER=1232
```

## Auteur

Projet réalisé dans le cadre du cours MGL849 - Systèmes Temps Réel