

## 👤 Personne 1 — Communication réseau

### `src/network/socket.c` + `include/network/socket.h`
- [ ] Adapter le socket existant pour la **réception** (en plus de l'envoi)
- [ ] Implémenter le parsing des messages entrants (`LG1Val`, `LG2Val`, `LG3Val`)
- [ ] Gérer la connexion/déconnexion proprement

### `src/tasks/task_receiver.c` (Thread T1 — priorité 90)
- [ ] Recevoir en boucle les messages du simulateur via le socket
- [ ] Extraire le numéro de gaz et la valeur (`LGxVal` → `gas_values[x]`)
- [ ] Verrouiller `mutex_gas`, écrire dans `gas_values`, déverrouiller
- [ ] Horodater `Tcapteur` pour les métriques
- [ ] Faire `sem_post(sem_new_data)` pour réveiller T2

### `src/tasks/task_sender.c` (Thread T3 — priorité 80)
- [ ] Attendre `sem_wait(sem_new_cmd)`
- [ ] Lire la commande dans `cmd_to_send`
- [ ] Envoyer la commande via socket (`AL1`, `VL2`, `IG1`, `AG1L`, etc.)
- [ ] Horodater `Taction` pour les métriques

---

## 👤 Personne 2 — Logique de décision

### `src/control/decision_engine.c` + `include/control/decision_engine.h`
- [ ] Implémenter le calcul du niveau d'alarme selon les seuils :
  - `0–5` → aucune alarme
  - `6–20` → `L`
  - `21–50` → `M`
  - `51–100` → `H`
- [ ] Implémenter la table de décision action/coût :
  - `L` → Aération niveau 1 (coût 1)
  - `M` → Ventilation niveau 1 ou 2 (coût 6 ou 9)
  - `H` → Injection de gaz (coût 20)
- [ ] Implémenter la logique d'annulation (`AN`, `VN`, `AIG1/2/3`) quand le niveau redescend
- [ ] Implémenter le calcul d'efficacité `Ei`

### `src/tasks/task_decision.c` (Thread T2 — priorité 85)
- [ ] Attendre `sem_wait(sem_new_data)`
- [ ] Lire `gas_values[3]` sous `mutex_gas`
- [ ] Appeler `decision_engine` pour chaque gaz
- [ ] Mettre à jour `alarm_levels[3]`
- [ ] Écrire la commande dans `cmd_to_send`
- [ ] Faire `sem_post(sem_new_cmd)` → réveille T3
- [ ] Faire `sem_post(sem_new_alarm)` → réveille T4

### `shared_data.c` (en collaboration avec Phase 0)
- [ ] Fonctions init/destroy pour mutex et sémaphores
- [ ] Fonctions get/set thread-safe pour `gas_values` et `alarm_levels`

---

## 👤 Personne 3 — Affichage & métriques

### `src/display/led_matrix.c` + `include/display/led_matrix.h`
- [ ] Initialiser la matrice LED du Sense Hat
- [ ] Implémenter l'affichage d'un caractère (`L`, `M`, `H`) avec couleur :
  - `L` → vert
  - `M` → orange
  - `H` → rouge
- [ ] Implémenter le clignotement (on/off avec `nanosleep`)
- [ ] Éteindre la matrice quand aucune alarme

### `src/tasks/task_display.c` (Thread T4 — priorité 50)
- [ ] Attendre `sem_wait(sem_new_alarm)`
- [ ] Lire `alarm_levels[3]`
- [ ] Appeler `led_matrix` avec le niveau le plus critique détecté
- [ ] Gérer la boucle de clignotement

### `src/metrics/metrics.c` + `include/metrics/metrics.h`
- [ ] Définir les structures pour stocker `Tcapteur`, `Taction`, `TRi`
- [ ] Implémenter le calcul de `TRp` (moyenne des TR sur une période)
- [ ] Implémenter le calcul de `Ei` (efficacité par période)
- [ ] Implémenter le calcul de `Ci` (coût cumulé des actions)
- [ ] Écrire les résultats dans `metrics.log`

### `src/tasks/task_metrics.c` (Thread T5 — SCHED_OTHER)
- [ ] Tourner en boucle avec `nanosleep` (ex. toutes les 10 secondes)
- [ ] Lire les timestamps accumulés
- [ ] Appeler les fonctions de calcul
- [ ] Logger les résultats

---

