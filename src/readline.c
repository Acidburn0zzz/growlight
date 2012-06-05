#include <assert.h>
#include <stdlib.h>
#include <locale.h>
#include <readline/history.h>
#include <readline/readline.h>

#include <growlight.h>

#define ZERO_ARG_CHECK(args) \
 if(args[1]){ fprintf(stderr,"Usage: %s\n",*args); return -1 ; }

static const char *
pcie_gen(unsigned gen){
	switch(gen){
		case 1: return "1.0";
		case 2: return "2.0";
		case 3: return "3.0";
		default: return "unknown";
	}
}

static int
print_controller(const controller *c){
	int r = 0,rr;

	switch(c->bus){
		case BUS_PCIe:
			rr = printf("PCI Express device %04hx:%02x.%02x.%x (x%u, gen %s)\n",
					c->pcie.domain,c->pcie.bus,
					c->pcie.dev,c->pcie.func,
					c->pcie.lanes_neg,pcie_gen(c->pcie.gen));
			if(rr < 0){
				return -1;
			}
			r += rr;
			break;
		default:
			fprintf(stderr,"Unknown bus type: %d\n",c->bus);
			return -1;
	}
	rr = printf("\t%s\n",c->name);
	if(rr < 0){
		return -1;
	}
	r += rr;
	return r;
}

static int
initiators(char * const *args){
	const controller *c;

	ZERO_ARG_CHECK(args);
	c = get_controllers();
	while(c){
		if(print_controller(c) < 0){
			return -1;
		}
		c = c->next;
	}
	return 0;
}

static void
free_tokes(char **tokes){
	char **toke;

	if(tokes){
		for(toke = tokes ; *toke ; ++toke){
			free(*toke);
		}
		free(tokes);
	}
}

static int
tokenize(const char *line,char ***tokes){
	int t = 0;

	*tokes = NULL;
	do{
		const char *s;
		char *n,**tmp;

		while(isspace(*line)){
			++line;
		}
		s = line;
		while(isgraph(*line)){
			++line;
		}
		if(line == s){
			break;
		}
		if((n = strndup(s,line - s)) == NULL){
			free_tokes(*tokes);
			return -1;
		}
		// Use t + 2 because we must have space for a final NULL
		if((tmp = realloc(*tokes,sizeof(**tokes) * (t + 2))) == NULL){
			free(n);
			free_tokes(*tokes);
			return -1;
		}
		*tokes = tmp;
		(*tokes)[t++] = n;
	}while(*line);
	if(t){
		(*tokes)[t] = NULL;
	}
	return t;
}

static int
tty_ui(void){
	const struct fxn {
		const char *cmd;
		int (*fxn)(char * const *);
	} fxns[] = {
#define FXN(x) { .cmd = #x, .fxn = x, }
		FXN(initiators),
		{ .cmd = NULL,		.fxn = NULL, },
#undef FXN
	};
	const char prompt[] = "[growlight]> ";
	char *l;

	// FIXME need command line completion!
	while( (l = readline(prompt)) ){
		const struct fxn *fxn;
		char **tokes;
		int z;

		z = tokenize(l,&tokes);
		free(l);
		if(z == 0){
			continue;
		}else if(z < 0){
			return -1;
		}
		if(strcasecmp(tokes[0],"quit") == 0){
			free_tokes(tokes);
			break;
		}
		if(strcasecmp(tokes[0],"help") == 0){
			free_tokes(tokes);
			printf("\n\tAvailable commands:\n\n");
			for(fxn = fxns ; fxn->cmd ; ++fxn){
				printf("\t  %s\n",fxn->cmd);
			}
			printf("\n");
			continue;
		}
		for(fxn = fxns ; fxn->cmd ; ++fxn){
			if(strcasecmp(fxn->cmd,tokes[0])){
				continue;
			}
			break;
		}
		if(fxn->fxn){
			fxn->fxn(tokes);
		}else{
			fprintf(stderr,"Unknown command: %s\n",tokes[0]);
		}
		free_tokes(tokes);
	}
	printf("\n");
	return 0;
}

int main(int argc,char * const *argv){
	if(growlight_init(argc,argv)){
		return EXIT_FAILURE;
	}
	rl_prep_terminal(1); // 1 == read 8-bit input
	if(tty_ui()){
		growlight_stop();
		return EXIT_FAILURE;
	}
	if(growlight_stop()){
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
