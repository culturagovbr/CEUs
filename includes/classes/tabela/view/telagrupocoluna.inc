	<table  class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="SubTituloCentro" colspan="2"><?= ("Grupo de Coluna"); ?></td>
		</tr>
		<tr>
			<td align='right' class="SubTituloDireita"><?= ("T�tulo do Grupo Coluna:"); ?></td>
			<td>
			<?
			echo campo_texto('titulogrupocoluna','N','','',46,100,'','');
			?>
			</td>
		</tr>
		<tr style="background-color: #cccccc">
			<td align='right' style="vertical-align:top; width:25%">&nbsp;</td>
			<td align='left' style="vertical-align:top; width:25%">�
				<input type="button" value="<?= ("Salvar"); ?>" onclick="TableManager.saveGrupoColuna();">
			</td>
		</tr>
	</table>
