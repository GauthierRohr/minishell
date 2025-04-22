// Parser inputs
//
//	Séparer la ligne en tokens (split sur les espaces en gérant les quotes).
//	Gérer les redirections (>, <, >>, <<).
//	Gérer les pipes (|).

/*
🧠 1. Phase de Tokenisation

Commence par transformer la ligne de commande en une liste de tokens en respectant les règles suivantes :
	•	Guillemets simples ' : tout ce qui est entre ' est un token unique, sans interprétation des variables ou des caractères spéciaux.
	•	Guillemets doubles " : le contenu est un token unique, mais les variables ($VAR) y sont interprétées.
	•	Caractères spéciaux : les symboles comme |, <, >, >>, << doivent être identifiés comme des tokens distincts.
	•	Espaces : utilise-les pour séparer les tokens, sauf lorsqu’ils sont à l’intérieur de guillemets.

Pour gérer cette phase, tu peux implémenter un automate fini qui lit caractère par caractère, en maintenant un état (par exemple : normal, dans des guillemets simples, dans des guillemets doubles) pour déterminer comment traiter chaque caractère.

⸻

🧱 2. Construction de la Structure de Commande

Une fois la tokenisation effectuée, organise les tokens en une structure reflétant la commande à exécuter :
	•	Commandes et arguments : identifie la commande principale et ses arguments.
	•	Redirections : associe les tokens de redirection (<, >, etc.) avec les fichiers correspondants.
	•	Pipes : sépare les différentes commandes reliées par des pipes (|) en sous-structures.

Cette structure te permettra de gérer l’exécution des commandes, les redirections et les pipes de manière organisée.

⸻

🔄 3. Évaluation des Variables et Expansion

Avant l’exécution, remplace les variables ($VAR, $?, etc.) par leurs valeurs actuelles. Assure-toi de ne pas effectuer cette expansion à l’intérieur des guillemets simples, conformément au comportement standard des shells.

⸻

🧪 4. Exécution des Commandes

Avec la structure de commande prête : ￼
	•	Commandes simples : exécute-les directement.
	•	Pipes : crée des pipes entre les processus et exécute les commandes dans le bon ordre.
	•	Redirections : utilise dup2 pour rediriger les entrées/sorties selon les indications. ￼ ￼

⸻

🧩 5. Gestion des Signaux

Implémente des gestionnaires pour les signaux comme SIGINT (Ctrl-C) et SIGQUIT (Ctrl-) pour que ton shell réagisse de manière appropriée, par exemple en interrompant l’exécution d’une commande ou en affichant une nouvelle invite.

⸻

📁 6. Organisation du Code

Pour une meilleure maintenabilité, sépare ton code en plusieurs fichiers :
	•	tokenizer.c : gestion de la tokenisation.
	•	parser.c : construction de la structure de commande.
	•	executor.c : exécution des commandes.
	•	signals.c : gestion des signaux.
	•	env.c : gestion des variables d’environnement.

Cette organisation te permettra de travailler plus efficacement et de faciliter le débogage.
*/
char	**split_input(char *input)
{

}