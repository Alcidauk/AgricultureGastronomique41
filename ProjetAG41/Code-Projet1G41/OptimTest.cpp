
void optimisation::optimisation_FrequencyTabuSearch(char *nom, int stable,
										 pointTest** lesPTA, int nb_tp_a,
										 secteur** lesSecteurA, int nb_secteur_a,int delta_tilt, int delta_power, int no_scen){
	GOutputFile file_sortie(nom);
    file_sortie.open();
	srand (time (NULL));
	clock_t startTime=clock();
	file_sortie << "Optimisation robuste des frequences avec tabu search" << "\n";
    file_sortie << "les parametres de l optimisation" << "\n";
    file_sortie <<"8h, 15h, 18h"<< "\n";
    file_sortie << "facteur de stabilite = " << stable << "\n";
    file_sortie << "le critere est: le nombre de clients non couverts"<< "\n";
    Table_sites* clostest_sites;
    double elapsedTime=0;
	double nb_clients_non_couvert = 0.0;
	double best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
	double best_nb_clients_non_couvert2 = best_nb_clients_non_couvert+1;
    int timeOfRun_sec = 200;
    //int saveSite = 40;
    std::vector <TabuItem*> tabuList;
    vector <TabuItem*>::iterator it;
    file_sortie << "Duree d execution: "<< timeOfRun_sec << "\n";
    file_sortie << "La solution initiale" << "\n";
    for (int i = 0; i<site::size; i++){
                for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
                {
                    site::lesSites[i]->get_secteur(j)->update_signal();
                    file_sortie << "le site :" << i << "le secteur :" << j << " la porteuse :" << site::lesSites[i]->get_secteur(j)->get_porteuse() << "\n";
                }

            }

        int nbIteration =0;
        int nbVoisinSite = 5;
        int aleaSite1 = rand()%36;
        int bestSite = -1;
        bool randSite = true;
        cout << "ici le debut de lalgorithme" << endl;
        /// to do tant que le temps d execution n est pas atteint
        while (elapsedTime<timeOfRun_sec){
            if (randSite == true){
            file_sortie << "le site choisi ESSSSSSSSSSSSSSSSSSSSSSSSST: " << aleaSite1 << "\n";
            file_sortie<< "----------------------------------------------------------------------" << "\n";
            file_sortie << "------------------Optimisation le site choisi-------------------------" << "\n";
            file_sortie << "----------------------------------------------------------------------" << "\n";
            file_sortie << "optimisation du site numero: " << aleaSite1 << "\n";

            if (site::lesSites[aleaSite1]->get_nb_secteur() == 1) {
            int old_p = site::lesSites[aleaSite1]->get_secteur(0)->get_porteuse();
            int best_p = old_p;
            for (int p=0; p<Environement::nbProteuses; p++) {
            if (p != old_p) {
                site::lesSites[aleaSite1]->get_secteur(0)->set_porteuse(p);
                site::lesSites[aleaSite1]->get_secteur(0)->update_signal();
                nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
                    if (best_nb_clients_non_couvert>nb_clients_non_couvert) {
                    file_sortie << (float)best_nb_clients_non_couvert << " -> " << (float)nb_clients_non_couvert << "la puissance "<<" (" << p << ")" << "\n";
                    file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                    file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                    file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                    file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
                    best_nb_clients_non_couvert=nb_clients_non_couvert;
                    best_p = p;
                    }
            }
            }
            if (best_p != Environement::nbProteuses-1){
            site::lesSites[aleaSite1]->get_secteur(0)->set_porteuse(best_p);
            site::lesSites[aleaSite1]->get_secteur(0)->update_signal();
            }
        }
        else {
        int k = 0, l = 1;
        int permutation = 3;
        int nb_permutations = MathB::fact(permutation)-1;
        int *best_p = (int*)malloc(permutation*sizeof(int));
        int *les_p = (int*)malloc(permutation*sizeof(int));
        for (int j=0; j<site::lesSites[aleaSite1]->get_nb_secteur(); j++)
        les_p[j] = site::lesSites[aleaSite1]->get_secteur(j)->get_porteuse();
            if (site::lesSites[aleaSite1]->get_nb_secteur()==2) les_p[2] = 2;
            for (int j=0; j<permutation; j++)
                best_p[j] = les_p[j];
                for (int ii=0; ii<nb_permutations; ii++) {
                    int tmp = les_p[k];
                    les_p[k] = les_p[l];
                    les_p[l] = tmp;
                    if (site::lesSites[aleaSite1]->get_nb_secteur()>k) site::lesSites[aleaSite1]->get_secteur(k)->set_porteuse(les_p[k]);
                    if (site::lesSites[aleaSite1]->get_nb_secteur()>l) site::lesSites[aleaSite1]->get_secteur(l)->set_porteuse(les_p[l]);
                    site::lesSites[aleaSite1]->get_secteur(k)->update_signal();
                    site::lesSites[aleaSite1]->get_secteur(l)->update_signal();
                    nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
                        if (best_nb_clients_non_couvert>nb_clients_non_couvert) {
                        file_sortie << "le site" << aleaSite1 << "\n";
                        file_sortie << (float)best_nb_clients_non_couvert << " --> " << (float)nb_clients_non_couvert << "\n";
                        file_sortie << "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << "\n";
                        file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                        file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                        file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                        file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
                        best_nb_clients_non_couvert=nb_clients_non_couvert;
                        for (int j=0; j<site::lesSites[aleaSite1]->get_nb_secteur(); j++)
                            best_p[j] = les_p[j];
                        }
                            // inverse k et l
                k = (k+1)%permutation;
                l = (l+1)%permutation;
                }
            for (int j=0; j<site::lesSites[aleaSite1]->get_nb_secteur(); j++){
            site::lesSites[aleaSite1]->get_secteur(j)->set_porteuse(best_p[j]);
            }
            //for (int j=0; j<site::lesSites[aleaSite1]->get_nb_secteur(); j++){
                //site::lesSites[aleaSite1]->get_secteur(j)->update_signal();
           // }
            // cout << "valid = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
            // cout << "f=" << best_p[0] << " " << best_p[1] << " " << best_p[2] << endl;
            free(best_p);
            free(les_p);
            }

            // Initialisation de la solution
            for (int i = 0; i<site::size; i++)
            {
                for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
                {
                    site::lesSites[i]->get_secteur(j)->update_signal();
                    file_sortie << "le site :" << i << "le secteur :" << j << " la porteuse :" << site::lesSites[i]->get_secteur(j)->get_porteuse() << "\n";
                }

            }
            best_nb_clients_non_couvert= Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
            bestSite = aleaSite1;
            }
                double nb_clients_non_couvert_voisin = 0.0;
                double best_nb_clients_non_couvert_voisin = best_nb_clients_non_couvert;
                double best2_nb_clients_non_couvert_voisin = best_nb_clients_non_couvert;
                //file_sortie << "le site a modifer: "<< aleaSite1 << "\n";
                TabuItem* myTabu = new TabuItem();
                nbIteration ++;
                file_sortie << "---------------------------------------------------------------------" << "\n";
                file_sortie << "Le nombre d iteration de l algo est: " << nbIteration << "\n";
                bool notTabu = true;
                Table_sites* voisins;
                //int aleaSite1 = rand()%36; // prendre un site au hasard
                file_sortie << "le site a modifier" << bestSite << "\n";
                int currentSite = bestSite;
                int pos = 0;
                for (it = tabuList.begin();it < tabuList.end(); ++it){
                    if ((*it)->no_site == bestSite)
                    {
                        if ((*it)->dureeTabu >= nbIteration){
                            file_sortie << "Yesssssssssssssssssssssssssssssssssssssssssssssss le site est tabou" <<  "\n";
                            file_sortie<< "duree tabou: " << (*it)->dureeTabu << " le nombre diteration: " << nbIteration << "\n";
                        notTabu = false;

                        }
                        if ((*it)->dureeTabu < nbIteration){
                            file_sortie << "nombre diteration depasse la duree tabou -> le site n'est plus tabou " << (*it)->dureeTabu << "\n";
                            tabuList.erase(tabuList.begin()+pos);
                        }
                    }
                    pos++;
                }
                //||(currentDureeTabu <= nbIteration))
                if(notTabu == true){
                    file_sortie << "le site n est pas tabou -> L ajouter a la liste tabou"<< "\n";
                    myTabu->no_site = bestSite;
                    myTabu->dureeTabu = Random::randDureeTabu(3,10)+ nbIteration;
                    tabuList.push_back(myTabu);
                    //int currentSite = aleaSite1;
                    file_sortie << "afficher la tabou liste " << "\n";
                    file_sortie << "la taille de la liste tabou " << (float)tabuList.size() << "\n";
                    for (it = tabuList.begin();it < tabuList.end(); ++it){
                        file_sortie << "le numero du site " << (*it)->no_site << " et duree tabou " << (*it)->dureeTabu << "\n";
                    }

                    /// Calcul du voisinage du site choisi
                    find_closest_site(bestSite, nbVoisinSite, voisins);
                    for (int k=0; k<nbVoisinSite; k++)
                    {
                        file_sortie << " le numero du voisin "<< (float)voisins[k].no_site << " et distance " << (float)voisins[k].dist_site_m << "\n";
                    }
                    file_sortie << "----------------------------------------------------------------------------" << "\n";
                    file_sortie << "------------------Optimisation du voisinage du site-------------------------" << "\n";
                    file_sortie << "----------------------------------------------------------------------------" << "\n";
                    //int permutation = 3;
                    //int nb_permutations = MathB::fact(permutation)-1;
                    //int *best_p = (int*)malloc(permutation*sizeof(int));
                    //int *les_p = (int*)malloc(permutation*sizeof(int));
                     //int posTabou =-1;
                     bool voisinsTabou[nbVoisinSite];
                     for (int i=0; i<nbVoisinSite; i++){
                     voisinsTabou[i]= false;
                     }
                    for (int i=0; i< nbVoisinSite; i++) {
                        if (tabuList.size()!=0){
                            for (it = tabuList.begin(); it < tabuList.end(); ++it){
                                if (((voisins[i].no_site)==((*it)->no_site)) && (((*it)->dureeTabu)>nbIteration)){
                                voisinsTabou[i] = true;
                                file_sortie << "**********STOP***********" << "\n";
                                file_sortie << "le site numero " << voisins[i].no_site << "est tabou, on va pas L optimiser " << "\n";
                                }
                            }
                        }
                   }
                   for (int i=0; i<nbVoisinSite; i++){
                     file_sortie << " est ce le" << i << " eme voisin est tabou :" << voisinsTabou[i] << "\n";
                     }

                    int permutation = 3;
                    int nb_permutations = MathB::fact(permutation)-1;
                    int *best_p = (int*)malloc(permutation*sizeof(int));
                    int *les_p = (int*)malloc(permutation*sizeof(int));
                    int *best_pp = (int*)malloc(permutation*sizeof(int));
                    int best1_p = -1;
                    for (int i=0; i< nbVoisinSite; i++) {
                        if (voisinsTabou[i]==false){
                            file_sortie << "optimisation du site numero: " << voisins[i].no_site << "\n";
                            file_sortie << "Optimisation sur les frequences" << "\n";
                            if (site::lesSites[voisins[i].no_site]->get_nb_secteur() == 1) {
                            int old_p = site::lesSites[voisins[i].no_site]->get_secteur(0)->get_porteuse();
	//			          best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
                            for (int p=0; p<Environement::nbProteuses; p++) {
                                if (p != old_p) {
                                    site::lesSites[voisins[i].no_site]->get_secteur(0)->set_porteuse(p);
//						             nb_clients_non_couvert = Fitness::eval(stable,lesSecteurA, nb_secteur_a, site::lesSites[i]->get_secteur(0));
                                    nb_clients_non_couvert_voisin = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
                                    if (best_nb_clients_non_couvert_voisin>nb_clients_non_couvert_voisin) {
                                        //cout << best_nb_clients_non_couvert_voisin << "Nbre clients non couverts -> " << nb_clients_non_couvert_voisin << endl;
                                        // cout << "le site -> "<< i << " Le secteur ->"<< j << "\n";
                                        file_sortie << (float)best_nb_clients_non_couvert_voisin << " -> " << (float)nb_clients_non_couvert_voisin << "la puissance "<<" (" << p << ")" << "\n";
                                        file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                                        file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                                        file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                                        file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
                                        //cout << best_nb_clients_non_couvert_voisin << " -> " << nb_clients_non_couvert_voisin << "la puissance "<<" (" << p << ")" << "\n";
                                        //cout << "non couverts t = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
                                        //cout << "non couverts 1 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << endl;
                                        //cout << "non couverts 2 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << endl;
                                        //cout << "non couverts 3 = " <<  Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << endl;
                                        best_nb_clients_non_couvert_voisin =nb_clients_non_couvert_voisin;
                                        bestSite = voisins[i].no_site;
                                        best1_p = p;
                                        }
                                }
                            }
                            //if (best_p != Environement::nbProteuses-1)
                            //site::lesSites[voisins[i].no_site]->get_secteur(0)->set_porteuse(best_p);
                        }
                        else {
                        int k = 0, l = 1;
                        for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                        les_p[j] = site::lesSites[voisins[i].no_site]->get_secteur(j)->get_porteuse();
                            if (site::lesSites[voisins[i].no_site]->get_nb_secteur()==2) les_p[2] = 2;
                            for (int j=0; j<permutation; j++)
                            best_pp[j] = les_p[j];
                                //best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
                                for (int ii=0; ii<nb_permutations; ii++) {
                                //cout << "indice : " << k << " " << l << endl;
                                //cout << "freque : " << les_p[k] << " " << les_p[l] << endl;
                                    int tmp = les_p[k];
                                    les_p[k] = les_p[l];
                                    les_p[l] = tmp;
                                    if (site::lesSites[voisins[i].no_site]->get_nb_secteur()>k) site::lesSites[voisins[i].no_site]->get_secteur(k)->set_porteuse(les_p[k]);
                                    if (site::lesSites[voisins[i].no_site]->get_nb_secteur()>l) site::lesSites[voisins[i].no_site]->get_secteur(l)->set_porteuse(les_p[l]);
                                    //site::lesSites[voisins[i].no_site]->get_secteur(k)->update_signal();
                                    //site::lesSites[voisins[i].no_site]->get_secteur(l)->update_signal();
                                    //nb_clients_non_couvert_voisin = Fitness::eval(stable, lesSecteurA, nb_secteur_a, site::lesSites[i]->get_secteur(k));
                                    nb_clients_non_couvert_voisin = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
                                    //cout << " -> " << nb_clients_non_couvert_voisin << endl;
                                        if (best_nb_clients_non_couvert_voisin >  nb_clients_non_couvert_voisin) {
                                        file_sortie << "le site" << voisins[i].no_site << "\n";
                                        file_sortie << (float)best_nb_clients_non_couvert_voisin << " --> " << (float)nb_clients_non_couvert_voisin << "\n";
                                        file_sortie << "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << "\n";
                                        file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                                        file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                                        file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                                        file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
                                        //cout << "le site" << voisins[i].no_site << "\n";
                                        //cout <<  best_nb_clients_non_couvert_voisin << " --> " << nb_clients_non_couvert_voisin << "\n";
                                        //cout << "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << "\n";
                                        //cout << "non couverts t = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                                        //cout << "non couverts 1 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                                        //cout << "non couverts 2 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                                        //cout << "non couverts 3 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
                                        best_nb_clients_non_couvert_voisin =nb_clients_non_couvert_voisin;
                                        bestSite = voisins[i].no_site;
                                        for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                            best_p[j] = les_p[j];
                                           // best2_p[j]=les_p[j]
                                        }
                                       // for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                           // {
                                            //cout << "le site :" << voisins[i].no_site << "le secteur :" << j << " la porteuse :" << site::lesSites[voisins[i].no_site]->get_secteur(j)->get_porteuse() << endl;
                                            //}
                                // inverse k et l
                                k = (k+1)%permutation;
                                l = (l+1)%permutation;
                                }

                                ///garder les autres sites qui améliore la fitness, autre que bestSite
                               /* for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                {
                                    if ((voisins[i].no_site)!= bestSite){
                                    site::lesSites[voisins[i].no_site]->get_secteur(j)->set_porteuse(best_pp[j]);
                                    }
                                }*/
                               // for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                //{
                                   // if ((voisins[i].no_site)!= bestSite){
                                    //site::lesSites[voisins[i].no_site]->get_secteur(j)->set_porteuse(best_pp[j]);
                                   // }
                               // }
                                //for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                //site::lesSites[aleaSite1]->get_secteur(j)->set_porteuse(best_pp[j]);
                            //for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                            //site::lesSites[voisins[i].no_site]->get_secteur(j)->set_porteuse(best_p[j]);
	//			              cout << "valid = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
	//			              cout << "f=" << best_p[0] << " " << best_p[1] << " " << best_p[2] << endl;
                            }
                           }// sortie de la boucle postabou = i

                        }


                        file_sortie << "le Best site est" << bestSite << "\n";

                        if (currentSite != bestSite){
                            for(int i=0; i< nbVoisinSite; i++)
                            {
                                if (voisinsTabou[i]==false){
                                for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                {
                                    if ((voisins[i].no_site)!= bestSite){
                                    site::lesSites[voisins[i].no_site]->get_secteur(j)->set_porteuse(best_pp[j]);
                                    //site::lesSites[voisins[i].no_site]->get_secteur(j)->update_signal();
                                    }
                                }
                                }
                            }
                            if (site::lesSites[bestSite]->get_nb_secteur() == 1)
                                {
                                site::lesSites[bestSite]->get_secteur(0)->set_porteuse(best1_p);

                                best_nb_clients_non_couvert = best_nb_clients_non_couvert_voisin;
                                }
                                else
                                {
                                for (int j=0; j<site::lesSites[bestSite]->get_nb_secteur(); j++){
                                    site::lesSites[bestSite]->get_secteur(j)->set_porteuse(best_p[j]);
                                    //site::lesSites[bestSite]->get_secteur(j)->update_signal();
                                    best_nb_clients_non_couvert = best_nb_clients_non_couvert_voisin;
                                    }
                                }
                                randSite = false;
                        }
                        else
                        {
                            for(int i=0; i< nbVoisinSite; i++)
                            {
                                if (voisinsTabou[i]==false){
                                for (int j=0; j<site::lesSites[voisins[i].no_site]->get_nb_secteur(); j++)
                                {
                                    site::lesSites[voisins[i].no_site]->get_secteur(j)->set_porteuse(best_pp[j]);
                                    //site::lesSites[voisins[i].no_site]->get_secteur(j)->update_signal();
                                }
                                }
                            }
                            bool notNeighbour = true;
                            while (notNeighbour){
                                notNeighbour = false;
                                bestSite = rand()%36;
                                for (int i=0; i< nbVoisinSite; i++)
                                {
                                    if (bestSite == voisins[i].no_site){
                                    notNeighbour = true;
                                    break;
                                    }
                                }
                            }
                            randSite = true;
                        }
                        free(best_p);
                        free(les_p);
                        for (int i = 0; i<site::size; i++)
                        {
                            for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
                            {
                                file_sortie << "le site :" << i << "le secteur :" << j << " la porteuse :" << site::lesSites[i]->get_secteur(j)->get_porteuse() << "\n";
                            }

                        }
                }
                // si le site pris est tabou => prendre un autre site pas tabou
                else
                {
                   /* bool isTabu = true;
                    while (isTabu){
                        isTabu = false;

                        for (it = tabuList.begin();it < tabuList.end(); ++it){
                            if (((*it)->no_site == aleaSite1)&&((*it)->dureeTabu >= nbIteration)){
                            isTabu = true;
                            break;
                            }
                        }
                    }*/
                randSite = true;
                aleaSite1 = rand()%36;
                }

                // prendre un nouveau site si le site deja généré est tabou...a rajouter ici

      elapsedTime=(clock()-startTime) / (double)CLOCKS_PER_SEC;
	}
        for (int i = 0; i<site::size; i++)
            {
                for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
                {
                    site::lesSites[i]->get_secteur(j)->update_signal();
                    file_sortie << "le site :" << i << "le secteur :" << j << " la porteuse :" << site::lesSites[i]->get_secteur(j)->get_porteuse() << "\n";
                }

            }

	 best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
	 file_sortie<< "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
     file_sortie<< "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
     file_sortie<< "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
     file_sortie<< "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";
	//for (it = tabuList.begin();it < tabuList.end(); ++it){
                    //cout << "le numero du site " << (*it)->no_site << endl;
                    //cout << "le numero du site " << (*it)->dureeTabu << endl;
            // }
    //cout << "la taille de la liste tabou " << tabuList.size() << endl;
    file_sortie.close();

}
