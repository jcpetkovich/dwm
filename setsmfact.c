void
setsmfact(const Arg *arg) {
	float sf;

	if(!arg || !selmon->lt[selmon->sellt]->arrange)
		return;
	sf = arg->sf < 1.0 ? arg->sf + selmon->smfact : arg->sf - 1.0;
	if(sf < 0 || sf > 0.9)
		return;
	selmon->smfact = sf;
	arrange(selmon);
}
