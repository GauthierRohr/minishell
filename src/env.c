// Gestion de l’environnement
//
// Dupliquer envp dans une structure à nous.
// Pouvoir l’éditer (export, unset) sans le modifier directement.
//
// ---------------------
//❓ “Dupliquer l’environnement”, ça veut dire quoi ?
// Quand tu fais un main(int argc, char **argv, char **envp), le troisième paramètre "envp"
// contient l’environnement du processus courant, donc des lignes comme :
//
// PATH=/usr/local/bin:/usr/bin
// USER=gauthier
// PWD=/Users/gauthier/Desktop/42/3/minishell
//
// Ce tableau envp est un char **, mais il est géré par le système, donc tu n’as pas le droit de le modifier directement
// (sinon comportement indéfini).
// 👉 Tu dois le copier dans ta propre structure pour pouvoir le modifier librement (pour les export, unset, etc.).
//
// ✅ Exemples de ce que ça te permet :
//	•	Quand tu fais export LANG=fr_FR.UTF-8, tu dois pouvoir ajouter/modifier une variable.
//	•	Quand tu fais unset PATH, tu dois la retirer de ta copie.
//	•	Quand tu exécutes une commande avec execve(), tu dois lui passer ta propre copie de l’environnement (parce que tu l’as modifiée).