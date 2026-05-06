#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_FRAMES 10
#define MAX_REFERENCES 100

//  CLOCK
typedef struct {
    int page;           // Número de página
    int use_bit;        // Bit de uso (1 = recientemente usado)
} FrameClock;

// FIFO 
int fifo(int frames_count, int references[], int ref_count) {
    int frames[MAX_FRAMES];
    int page_faults = 0;
    int current_index = 0;
    
    // Inicializar marcos como vacíos (-1)
    for (int i = 0; i < frames_count; i++) {
        frames[i] = -1;
    }
    
    printf("\n--- FIFO Algorithm ---\n");
    printf("Ref\tFrames\t\t\tPage Fault?\n");
    printf("---\t------\t\t\t-----------\n");
    
    for (int i = 0; i < ref_count; i++) {
        int page = references[i];
        bool found = false;
        
        // Verificar si la página ya está en memoria
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == page) {
                found = true;
                break;
            }
        }
        
        // Page fault
        if (!found) {
            frames[current_index] = page;
            current_index = (current_index + 1) % frames_count;
            page_faults++;
            
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tYES (Fault #%d)\n", page_faults);
        } else {
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tNO\n");
        }
    }
    
    return page_faults;
}

// LRU 
int lru(int frames_count, int references[], int ref_count) {
    int frames[MAX_FRAMES];
    int last_used[MAX_FRAMES];  // Timestamp de último uso
    int page_faults = 0;
    int time_counter = 0;
    
    // Inicializar
    for (int i = 0; i < frames_count; i++) {
        frames[i] = -1;
        last_used[i] = -1;
    }
    
    printf("\n--- LRU Algorithm ---\n");
    printf("Ref\tFrames\t\t\tPage Fault?\n");
    printf("---\t------\t\t\t-----------\n");
    
    for (int i = 0; i < ref_count; i++) {
        int page = references[i];
        bool found = false;
        int frame_index = -1;
        
        // Verificar si la página ya está en memoria
        for (int j = 0; j < frames_count; j++) {
            if (frames[j] == page) {
                found = true;
                frame_index = j;
                break;
            }
        }
        
        if (found) {
            // Actualizar timestamp de la página encontrada
            last_used[frame_index] = time_counter++;
            
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tNO\n");
        } else {
            // Page fault - encontrar la página menos recientemente usada
            int lru_index = 0;
            for (int j = 1; j < frames_count; j++) {
                if (last_used[j] < last_used[lru_index]) {
                    lru_index = j;
                }
            }
            
            frames[lru_index] = page;
            last_used[lru_index] = time_counter++;
            page_faults++;
            
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j] != -1)
                    printf("%d ", frames[j]);
                else
                    printf("- ");
            }
            printf("\t\tYES (Fault #%d)\n", page_faults);
        }
    }
    
    return page_faults;
}

// CLOCK (Segundo intento)
int clock_algorithm(int frames_count, int references[], int ref_count) {
    FrameClock frames[MAX_FRAMES];
    int page_faults = 0;
    int hand = 0;  // Manecilla del reloj
    
    // Inicializar marcos
    for (int i = 0; i < frames_count; i++) {
        frames[i].page = -1;
        frames[i].use_bit = 0;
    }
    
    printf("\n--- CLOCK Algorithm ---\n");
    printf("Ref\tFrames (Page:UseBit)\t\tPage Fault?\n");
    printf("---\t--------------------\t\t-----------\n");
    
    for (int i = 0; i < ref_count; i++) {
        int page = references[i];
        bool found = false;
        int found_index = -1;
        
        // Verificar si la página ya está en memoria
        for (int j = 0; j < frames_count; j++) {
            if (frames[j].page == page) {
                found = true;
                found_index = j;
                break;
            }
        }
        
        if (found) {
            // Page hit - dar segunda oportunidad (set use_bit = 1)
            frames[found_index].use_bit = 1;
            
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j].page != -1)
                    printf("[%d:%d] ", frames[j].page, frames[j].use_bit);
                else
                    printf("[-:-] ");
            }
            printf("\t\tNO\n");
        } else {
            // Page fault - buscar página para reemplazar usando el algoritmo del reloj
            while (true) {
                if (frames[hand].use_bit == 0) {
                    // Encontró página sin bit de uso -> reemplazar
                    frames[hand].page = page;
                    frames[hand].use_bit = 1;  // Dar bit de uso a la nueva página
                    hand = (hand + 1) % frames_count;
                    break;
                } else {
                    // Dar segunda oportunidad y mover manecilla
                    frames[hand].use_bit = 0;
                    hand = (hand + 1) % frames_count;
                }
            }
            
            page_faults++;
            
            printf("%d\t", page);
            for (int j = 0; j < frames_count; j++) {
                if (frames[j].page != -1)
                    printf("[%d:%d] ", frames[j].page, frames[j].use_bit);
                else
                    printf("[-:-] ");
            }
            printf("\t\tYES (Fault #%d)\n", page_faults);
        }
    }
    
    return page_faults;
}

int main() {
    int frames_count;
    int ref_count;
    int references[MAX_REFERENCES];
    
    printf("========================================\n");
    printf("  PAGE REPLACEMENT ALGORITHMS SIMULATOR\n");
    printf("  FIFO | LRU | CLOCK\n");
    printf("========================================\n\n");
    
    // Solicitar número de marcos de página
    printf("Enter number of page frames (1-%d): ", MAX_FRAMES);
    scanf("%d", &frames_count);
    
    if (frames_count < 1 || frames_count > MAX_FRAMES) {
        printf("Invalid number of frames. Using default (3).\n");
        frames_count = 3;
    }
    
    // Solicitar secuencia de referencias
    printf("Enter number of page references (1-%d): ", MAX_REFERENCES);
    scanf("%d", &ref_count);
    
    if (ref_count < 1 || ref_count > MAX_REFERENCES) {
        printf("Invalid number of references. Using default (10).\n");
        ref_count = 10;
    }
    
    printf("\nEnter the page reference sequence (space separated):\n");
    for (int i = 0; i < ref_count; i++) {
        scanf("%d", &references[i]);
    }
    
    // Ejecutar los tres algoritmos
    printf("\n\n========================================\n");
    printf("              RESULTS\n");
    printf("========================================\n");
    
    int fifo_faults = fifo(frames_count, references, ref_count);
    int lru_faults = lru(frames_count, references, ref_count);
    int clock_faults = clock_algorithm(frames_count, references, ref_count);
    
    // Mostrar resumen comparativo
    printf("\n\n========================================\n");
    printf("           SUMMARY\n");
    printf("========================================\n");
    printf("Algorithm\tPage Faults\tHit Rate\n");
    printf("---------\t-----------\t--------\n");
    printf("FIFO\t\t%d\t\t%.2f%%\n", fifo_faults, 
           (1.0 - (float)fifo_faults/ref_count) * 100);
    printf("LRU\t\t%d\t\t%.2f%%\n", lru_faults,
           (1.0 - (float)lru_faults/ref_count) * 100);
    printf("CLOCK\t\t%d\t\t%.2f%%\n", clock_faults,
           (1.0 - (float)clock_faults/ref_count) * 100);
    
    printf("\n========================================\n");
    printf("Press Enter to exit...");
    getchar();  // Consumir el newline
    getchar();  // Esperar Enter
    
    return 0;
}